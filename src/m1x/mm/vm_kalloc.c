/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/param.h>
#include <kern/panic.h>
#include <hal/cpu.h>
#include <mm/kalloc.h>
#include <mm/vmem.h>
#include <mm/physmem.h>
#include <hal/param.h>
#include <lib/printf.h>
#include <string.h>

#define pr_trace(fmt, ...)  \
    printf("kalloc: " fmt, ##__VA_ARGS__)

/* Minimum allocation size */
#define MIN_LOG2_ALLOCSZ 3

/* Precomputed granularity table */
#define GRAN(x) (1 << (MIN_LOG2_ALLOCSZ + (x)))
static size_t grantab[MAGS_PER_WELL] = {
    GRAN(0), GRAN(1),
    GRAN(2), GRAN(3),
    GRAN(4), GRAN(5),
    GRAN(6), GRAN(7)
};

/*
 * Obtain a magazine well from a size
 */
static struct kalloc_mag *
kalloc_size_to_mag(struct kalloc_magwell *well, size_t n)
{
    if (well == NULL) {
        return NULL;
    }

    if (n >= grantab[7])
        return &well->mag[7];
    if (n >= grantab[6])
        return &well->mag[6];
    if (n >= grantab[5])
        return &well->mag[5];
    if (n >= grantab[4])
        return &well->mag[4];
    if (n >= grantab[3])
        return &well->mag[3];
    if (n >= grantab[2])
        return &well->mag[2];
    if (n >= grantab[1])
        return &well->mag[1];
    if (n >= grantab[0])
        return &well->mag[0];

    return NULL;
}

/*
 * Initialize a slab descriptor
 *
 * @desc: Slab descriptor to initialize
 * @mag:  Magazine we belong to
 *
 * Returns zero on success
 */
static int
kalloc_init_slabdesc(struct kalloc_slab_desc *desc, struct kalloc_mag *mag)
{
    uintptr_t pma;

    if (desc == NULL) {
        return -1;
    }

    pma = pmm_alloc_frame(PAGES_PER_SLAB);
    if (pma == 0) {
        panic("out of memory\n");
    }

    desc->slab = pma_to_vma(pma);
    desc->bitmap = 0;
    desc->mag = mag;
    return 0;
}

/*
 * Initialize a magazine
 *
 * @mag: Magazine to initialize
 * @index: Index of magazine
 */
static int
kalloc_init_mag(struct kalloc_mag *mag, size_t index)
{
    uintptr_t desc_pool_pma;
    size_t desc_pool_entries;
    struct kalloc_slab_desc *desc_pool;

    if (mag == NULL) {
        return -1;
    }

    TAILQ_INIT(&mag->empty);
    TAILQ_INIT(&mag->partial);
    TAILQ_INIT(&mag->full);

    desc_pool_pma = pmm_alloc_frame(SLAB_DESC_POOLSZ);
    if (desc_pool_pma == 0) {
        return -1;
    }

    desc_pool = pma_to_vma(desc_pool_pma);
    desc_pool_entries = (PAGESIZE * SLAB_DESC_POOLSZ);
    desc_pool_entries /= sizeof(struct kalloc_slab_desc);
    mag->n_entries = desc_pool_entries;
    mag->gran = GRAN(index);

    for (size_t i = 0; i < desc_pool_entries; ++i) {
        kalloc_init_slabdesc(&desc_pool[i], mag);
        TAILQ_INSERT_TAIL(&mag->empty, &desc_pool[i], link);
    }

    return 0;
}

/*
 * Pull from an empty mag
 */
static struct kalloc_slab_desc *
kalloc_pull_empty(struct kalloc_mag *mag)
{
    struct kalloc_slab_desc *desc;

    if (mag == NULL) {
        return NULL;
    }

    if (TAILQ_EMPTY(&mag->empty)) {
        return NULL;
    }

    /* TODO: LOCKING */
    desc = TAILQ_FIRST(&mag->empty);
    TAILQ_REMOVE(&mag->empty, desc, link);
    TAILQ_INSERT_TAIL(&mag->partial, desc, link);
    return desc;
}

/*
 * Pull some slabs from a partial mag, if that fails, try
 * an empty one
 */
static struct kalloc_slab_desc *
kalloc_pull(struct kalloc_mag *mag)
{
    struct kalloc_slab_desc *desc;

    if (mag == NULL) {
        return NULL;
    }

    if (TAILQ_EMPTY(&mag->partial)) {
        if ((desc = kalloc_pull_empty(mag)) == NULL)
            return NULL;

        return desc;
    }

    /* TODO: LOCKING */
    desc = TAILQ_FIRST(&mag->partial);
    TAILQ_REMOVE(&mag->partial, desc, link);
    return desc;
}

/*
 * Allocate memory from a descriptor
 *
 * @desc:  Descriptor to allocate from
 * @n:     Number of bytes to allocate
 * @gran:  Magazine granularity
 */
static void *
kalloc_from_desc(struct kalloc_slab_desc *desc, size_t n, size_t gran)
{
    ssize_t bit_base = -1;
    size_t bits_needed;
    size_t bits_found = 0;
    void *base = NULL;

    if (desc == NULL) {
        return NULL;
    }

    bits_needed = n / gran;
    for (size_t i = 0; i < sizeof(desc->bitmap) * 8; ++i) {
        if (!ISSET(desc->bitmap, BIT(i))) {
            if (bit_base < 0) bit_base = i;
            ++bits_found;
        } else {
            bit_base = -1;
        }

        if (bits_found >= bits_needed) {
            base = PTR_OFFSET(desc->slab, bit_base * gran);
            break;
        }
    }

    for (size_t i = 0; i < bits_found; ++i) {
        desc->bitmap |= BIT(bit_base + i);
    }

    return base;
}

/*
 * Allocate a number of bytes from a well
 *
 * @well: Well to allocate from
 * @n:    Number of bytes to allocate
 * @res:  Descriptor result is written here
 *
 * Returns the base of the memory on success
 */
static void *
mm_well_kalloc(struct kalloc_magwell *well, size_t n, struct kalloc_slab_desc **res)
{
    struct kalloc_mag *mag;
    struct kalloc_slab_desc *desc;
    void *base;

    if (well == NULL || desc == NULL) {
        return NULL;
    }

    mag = kalloc_size_to_mag(well, n);
    if (mag == NULL) {
        return NULL;
    }

    desc = kalloc_pull(mag);
    base = kalloc_from_desc(desc, n, mag->gran);

    if (base == NULL) {
        return NULL;
    }

    *res = desc;
    return base;
}

void *
mm_kalloc(size_t size)
{
    struct kpcr *kpcr;
    struct kalloc_slab_desc *desc;
    struct kalloc_header *hdr;
    size_t data_off;
    void *mem, *data;

    if ((kpcr = hal_this_cpu()) == NULL) {
        return NULL;
    }

    size = ALIGN_UP(size, 1 << MIN_LOG2_ALLOCSZ);
    data_off = sizeof(struct kalloc_header);
    mem = mm_well_kalloc(
        &kpcr->magwell,
        size + data_off,
        &desc
    );

    if (mem == NULL) {
        return NULL;
    }

    /*
     * Zero the data part and leave the header part untouched as we
     * are going to overwrite it anyways
     */
    data = PTR_OFFSET(mem, data_off);
    memset(data, 0, size);

    /* Tack the header at the start */
    hdr = (struct kalloc_header *)mem;
    hdr->desc = desc;
    hdr->len = size;
    return data;
}

void
mm_kfree(void *ptr)
{
    size_t delta, off;
    struct kalloc_header *hdr;
    struct kalloc_slab_desc *desc;
    struct kalloc_mag *mag;

    if (ptr == NULL) {
        return;
    }

    hdr = PTR_NOFFSET(ptr, sizeof(struct kalloc_header));
    desc = hdr->desc;
    mag = desc->mag;

    /* Compute the bit offset and mark as free */
    delta = (size_t)ptr - (uintptr_t)desc->slab;
    for (size_t i = 0; i < hdr->len / mag->gran; ++i) {
        off = (delta / mag->gran) + i;
        desc->bitmap &= ~BIT(off);
    }
}

int
mm_kalloc_init(struct kalloc_magwell *well)
{
    if (well == NULL) {
        return -1;
    }

    for (size_t i = 0; i < MAGS_PER_WELL; ++i) {
        kalloc_init_mag(&well->mag[i], i);
    }

    return 0;
}
