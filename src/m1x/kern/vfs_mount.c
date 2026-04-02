/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/types.h>
#include <kern/mount.h>
#include <mm/kalloc.h>

int
vfs_mountlist_alloc(struct mountlist **res)
{
    struct mountlist *mlist;

    if (res == NULL) {
        return -1;
    }

    mlist = mm_kalloc(sizeof(*mlist));
    if (mlist == NULL) {
        return -1;
    }

    TAILQ_INIT(&mlist->list);
    mlist->count = 0;
    *res = mlist;
    return 0;
}
