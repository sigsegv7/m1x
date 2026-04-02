/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_MOUNT_H_
#define _KERN_MOUNT_H_ 1

#include <sys/queue.h>
#include <kern/vnode.h>
#include <sys/types.h>
#include <sys/limits.h>

/* Maximum fs_info name length */
#define FS_NAME_MAX 16

/*
 * Represents a system mountpoint
 *
 * @name: Mount name
 * @vp:   Vnode of mountpoint directory
 * @link: Mount list link
 */
struct mountpoint {
    char name[NAME_MAX];
    struct vnode *vp;
    TAILQ_ENTRY(mountpoint) link;
};

/*
 * Represents a mountlist that holds one or more
 * system mountpoints. These exist locally per process
 *
 * @list: Mount list
 * @count: Number of mountpoints
 */
struct mountlist {
    TAILQ_HEAD(, mountpoint) list;
    size_t count;
};

/*
 * Arguments associated with a mount operation
 *
 * @fs_name: Filesystem name max
 * @vp_res:  Vnode result is written here
 */
struct vfs_mount_args {
    char fs_name[FS_NAME_MAX];
    struct vnode **vp_res;
};

/*
 * Represents virtual file system operations that can be
 * performed on a whole filesystem.
 */
struct vfsops {
    int(*mount)(struct vfs_mount_args *args);
};

/*
 * Describes a single filesystem
 *
 * @name: File system name
 * @vfsops: Filesystem operations
 */
struct fs_info {
    char name[FS_NAME_MAX];
    struct vfsops *vfsops;
};

/*
 * Allocate a new mountlist
 *
 * @res: Result is written here
 *
 * Returns zero on success
 */
int vfs_mountlist_alloc(struct mountlist **res);

#endif /* !_KERN_MOUNT_H_ */
