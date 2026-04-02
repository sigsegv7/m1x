/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#include <kern/process.h>
#include <kern/mount.h>
#include <string.h>

/* PID ticker */
static volatile size_t next_pid = 0;

int
process_init(struct process *process, int flags)
{
    int error;

    if (process == NULL) {
        return -1;
    }

    memset(process, 0, sizeof(*process));
    process->pid = next_pid++;
    error = vfs_mountlist_alloc(&process->mountlist);
    if (error < 0) {
        return -1;
    }

    return 0;
}
