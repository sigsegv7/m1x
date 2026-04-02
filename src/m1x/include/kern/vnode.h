/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_VNODE_H_
#define _KERN_VNODE_H_ 1

#include <sys/types.h>

struct vnode;

/*
 * Represents valid vnode types
 *
 * @VNODE_NONE: This vnode has no type
 * @VNODE_REG:  This vnode represents a regular file
 * @VNODE_DEV:  This vnode represents a device file
 */
typedef enum {
    VNODE_NONE,
    VNODE_REG,
    VNODE_DEV
} vnode_type_t;

/*
 * Arguments used for the locate() vop
 *
 * @dirvp: Vnode of directory to look up win
 * @name:  Name to look up
 */
struct vop_locate_args {
    struct vnode *dirvp;
    const char *name;
};

/*
 * Vnode operations
 */
struct vops {
    int(*locate)(struct vop_locate_args *args);
};

/*
 * An abstract representation of a file
 *
 * @type: Vnode type
 * @vops: Vnode operations
 * @data: Filesystem specific data
 * @ref:  Reference count
 */
struct vnode {
    vnode_type_t type;
    struct vops *vops;
    void *data;
    int ref;
};

/*
 * Allocate a new vnode and a status code
 *
 * @type: Type to assign to vnode
 * @res:  Vnode result is written here
 *
 * Returns zero on success
 */
int vfs_vnode_alloc(vnode_type_t type, struct vnode **res);

#endif /* !_KERN_VNODE_H_ */
