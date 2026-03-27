/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <dev/cons/cons.h>
#include <lib/limine.h>
#include <flanterm.h>
#include <flanterm_backends/fb.h>

#define FRAMEBUFFER fbresp->framebuffers[0]

static struct flanterm_context *ft_ctx = NULL;
static struct limine_framebuffer_response *fbresp = NULL;
static struct limine_framebuffer_request fbreq = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void
cons_init(struct cons_attr *attr)
{
    if (attr == NULL) {
        return;
    }

    fbresp = fbreq.response;
    ft_ctx = flanterm_fb_init(
        NULL,
        NULL,
        FRAMEBUFFER->address,
        FRAMEBUFFER->width,
        FRAMEBUFFER->height,
        FRAMEBUFFER->pitch,
        FRAMEBUFFER->red_mask_size,
        FRAMEBUFFER->red_mask_shift,
        FRAMEBUFFER->green_mask_size,
        FRAMEBUFFER->green_mask_shift,
        FRAMEBUFFER->blue_mask_size,
        FRAMEBUFFER->blue_mask_shift,
        NULL, NULL, NULL,
        &attr->bg, &attr->fg, NULL,
        NULL, NULL, 0, 0, 0,
        0, 0, 0, 0
    );
}

void
cons_write(const char *s, size_t len)
{
    if (fbresp == NULL) {
        return;
    }

    if (s == NULL || len == 0) {
        return;
    }

    flanterm_write(ft_ctx, s, len);
}
