#ifndef __SCHED_SWITCH_H__
#define __SCHED_SWITCH_H__

#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

int handle_sched_switch()
{
    int pid = bpf_get_current_pid_tgid() >> 32;
    char comm[16];
    bpf_get_current_comm(comm, 16);

    bpf_printk("Hello from %s, pid=%d\n", comm, pid);

    return 0;
}

#endif // __SCHED_SWITCH_H__