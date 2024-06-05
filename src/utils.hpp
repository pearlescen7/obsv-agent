#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <string>

namespace utils {
    inline void log_error(const std::string& msg) {
        fprintf(stderr, "Error: %s (%d: %s)\n", msg, errno, strerror(errno));
    }
};