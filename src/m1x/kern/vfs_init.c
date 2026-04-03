/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <sys/param.h>
#include <kern/vnode.h>
#include <kern/mount.h>
#include <lib/printf.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("vfs: " fmt, ##__VA_ARGS__)

static struct fs_info fs_tab[] = {
    { "tmpfs", &g_tmpfs_vfsops }
};

int
vfs_init(void)
{
    struct fs_info *info;
    struct vfsops *ops;
    size_t n_error, n_init;

    n_error = 0;
    n_init = 0;

    for (size_t i = 0; i < NELEM(fs_tab); ++i) {
        info = &fs_tab[i];
        ops = info->vfsops;

        if (ops->init == NULL) {
            pr_trace("error: fs %s has no init callback\n", info->name);
            ++n_error;
            continue;
        }

        if (ops->init(info) < 0) {
            pr_trace("error: failed to initialize %s\n", info->name);
            ++n_error;
            continue;
        }

        ++n_init;
    }

    pr_trace("initialized %d filesystem(s), %d error(s)\n", n_init, n_error);
}

struct fs_info *
vfs_by_name(const char *name)
{
    struct fs_info *info;

    if (name == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < NELEM(fs_tab); ++i) {
        info = &fs_tab[i];
        if (*name != info->name[0]) {
            continue;
        }

        if (strcmp(name, info->name) != 0) {
            continue;
        }

        return info;
    }

    return NULL;
}
