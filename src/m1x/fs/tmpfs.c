/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/mount.h>
#include <kern/vnode.h>

/*
 * Tmpfs mount callback
 */
static int
tmpfs_mount(struct vfs_mount_args *args)
{
    if (args == NULL) {
        return -1;
    }

    return 0;
}

/*
 * Initialization callback
 */
static int
tmpfs_init(struct fs_info *info)
{
    if (info == NULL) {
        return -1;
    }

    return 0;
}

struct vfsops g_tmpfs_vfsops = {
    .mount = tmpfs_mount,
    .init = tmpfs_init
};
