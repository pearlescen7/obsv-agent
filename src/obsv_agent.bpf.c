#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "bpf_progs.h"

char _license[] SEC("license") = "GPL";

/////////////////////////////
// BPF Program definitions //
/////////////////////////////

SEC("tp/sched/sched_switch")
int sched_switch()
{
    return handle_sched_switch();
}