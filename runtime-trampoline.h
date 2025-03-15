#pragma once

#include <mach-o/loader.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void runtime_trampoline(int dyld_fd, struct mach_header_64 *dyld_mh, int exe_fd,
                               struct mach_header_64 *exe_mh, int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
