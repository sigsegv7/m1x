/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/vnode.h>
#include <mm/kalloc.h>
#include <string.h>

int
vfs_vnode_alloc(vnode_type_t type, struct vnode **res)
{
    struct vnode *vp;

    if (res == NULL) {
        return -1;
    }

    vp = mm_kalloc(sizeof(*vp));
    if (vp == NULL) {
        return -1;
    }

    memset(vp, 0, sizeof(*vp));
    vp->ref = 1;
    vp->type = type;
    *res = vp;
    return 0;
}
