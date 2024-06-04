#include <stdio.h>
#include <errno.h>
#include <string.h>

static void log_error(const char *msg) {
    fprintf(stderr, "Error: %s (%d: %s)\n", msg, errno, strerror(errno));
}

{
    {"sched_switch" : true}
    {"mem_usage" : false}
} 