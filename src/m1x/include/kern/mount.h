/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_MOUNT_H_
#define _KERN_MOUNT_H_ 1

#include <sys/queue.h>
#include <sys/vnode.h>
#include <sys/types.h>
#include <sys/limits.h>

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

#endif /* !_KERN_MOUNT_H_ */
