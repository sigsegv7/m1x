/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_VFS_H_
#define _KERN_VFS_H_ 1

#include <kern/mount.h>

/*
 * Initialize the virtual filesystem
 */
int vfs_init(void);

/*
 * Obtain a filesystem descriptor by name
 *
 * @name: Name of filesystem to obtain
 */
struct fs_info *vfs_by_name(const char *name);

#endif /* !_KERN_VFS_H_ */
