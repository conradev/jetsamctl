//
//  main.c
//  jetsamctl
//
//  Created by Conrad Kramer on 3/17/15.
//  Copyright (c) 2015 Kramer Software Productions, LLC. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>

#include <sys/sysctl.h>
#include <sys/kern_memorystatus.h>

int main(int argc, const char * argv[]) {
    static const char *usage = "usage: %s [-l limit] [-p priority] process\n";

    static struct option opts[] = {
        {"limit", optional_argument, NULL, 'l'},
        {"priority", optional_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };

    int ch;
    char *prioritystr = NULL, *limitstr = NULL;
    while ((ch = getopt_long(argc, (char * const *)argv, "l:p:", opts, NULL)) != -1) {
        switch (ch) {
            case 'l':
                limitstr = optarg;
                break;
            case 'p':
                prioritystr = optarg;
                break;
        }
    }

    const char *jetsamctl = argv[0];
    argc -= optind;
    argv += optind;

    if (argc == 0 || (prioritystr == NULL && limitstr == NULL)) {
        fprintf(stderr, usage, jetsamctl);
        return 1;
    }

    char *endptr;
    const char *process = argv[0];
    pid_t pid = strtoimax(process, &endptr, 0);
    if (process == endptr || *endptr != '\0') {
        int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};

        size_t size;
        if (sysctl(mib, 4, NULL, &size, NULL, 0) == -1) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(errno));
            return 1;
        }

        struct kinfo_proc *processes = malloc(size);
        if (processes == NULL) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(errno));
            return 1;
        }

        if (sysctl(mib, 4, processes, &size, NULL, 0) == -1) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(errno));
            return 1;
        }

        for (unsigned long i = 0; i < size / sizeof(struct kinfo_proc); i++) {
            if (strcmp(processes[i].kp_proc.p_comm, process) == 0) {
                pid = processes[i].kp_proc.p_pid;
                break;
            }
        }

        if (pid == 0) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(ESRCH));
            return 1;
        }
    }

    if (limitstr) {
        uint32_t limit = strtoimax(limitstr, &endptr, 0);
        if (limitstr == endptr || *endptr != '\0') {
            fprintf(stderr, "%s: %s: Invalid limit\n", jetsamctl, limitstr);
            return 1;
        }

        if (memorystatus_control(MEMORYSTATUS_CMD_SET_JETSAM_TASK_LIMIT, pid, limit, NULL, 0) == -1) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(errno));
            return 1;
        }
    }

    if (prioritystr) {
        uint32_t priority = strtoimax(prioritystr, &endptr, 0);
        if (prioritystr == endptr || *endptr != '\0') {
            fprintf(stderr, "%s: %s: Invalid priority\n", jetsamctl, prioritystr);
            return 1;
        }

        memorystatus_priority_entry_t properties;
        memset(&properties, 0, sizeof(memorystatus_priority_entry_t));
        properties.pid = pid;
        properties.priority = priority;

        if (memorystatus_control(MEMORYSTATUS_CMD_GRP_SET_PROPERTIES, 0, 0, &properties, sizeof(memorystatus_priority_entry_t)) == -1) {
            fprintf(stderr, "%s: error: %s\n", jetsamctl, strerror(errno));
            return 1;
        }
    }

    return 0;
}
