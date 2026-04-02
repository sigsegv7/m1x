/*
 * Copyright (c) 2026, Mirocom Laboratories
 * Provided under the BSD-3 clause
 */

#ifndef _KERN_PROCESS_H_
#define _KERN_PROCESS_H_ 1

#include <sys/types.h>

/*
 * Per-process descriptor
 *
 * @pid:  Process ID
 * @mountlist: Local mountlist
 */
struct process {
    pid_t pid;
    struct mountlist *mountlist;
};

/*
 * Initialize a system process
 *
 * @process:  Process descriptor
 * @flags:    Optional flags
 *
 * Returns zero on success
 */
int process_init(struct process *process, int flags);

#endif /* !_KERN_PROCESS_H_ */
