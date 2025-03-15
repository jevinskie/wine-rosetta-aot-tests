#undef NDEBUG
#include <assert.h>

#include <fcntl.h>
#include <inttypes.h>
#include <mach-o/loader.h>
#include <mach/mach.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "runtime-trampoline.h"

static void *mmap_file(const char *path, int prot, size_t *size, int *fd) {
    int mfd = open(path, O_RDONLY);
    if (mfd == -1) {
        fprintf(stderr, "open %s", path);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    if (fstat(mfd, &st) != 0) {
        fprintf(stderr, "fstat %s", path);
        exit(EXIT_FAILURE);
    }
    if (size)
        *size = (size_t)st.st_size;
    void *ptr = mmap(NULL, st.st_size, prot, MAP_PRIVATE, mfd, 0);
    if (ptr == NULL) {
        fprintf(stderr, "mmap");
        exit(EXIT_FAILURE);
    }
    if (fd)
        *fd = mfd;
    return ptr;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "argc < 2\n");
        return EXIT_FAILURE;
    }
    printf("calling dylibify_entry bye bye world\n");
    fflush(stdout);
    int dyld_fd    = -1;
    size_t dyld_sz = 0;
    struct mach_header_64 *dyld_mh =
        mmap_file("/usr/lib/dyld", PROT_READ | PROT_WRITE, &dyld_sz, &dyld_fd);
    assert(dyld_fd != -1);
    assert(dyld_sz != 0);
    int exe_fd                    = -1;
    size_t exe_sz                 = 0;
    struct mach_header_64 *exe_mh = mmap_file(argv[1], PROT_READ | PROT_WRITE, &exe_sz, &exe_fd);
    assert(exe_fd != -1);
    assert(exe_sz != 0);
    runtime_trampoline(dyld_fd, dyld_mh, exe_fd, exe_mh, argc - 1, &argv[1]);
    close(exe_fd);
    close(dyld_fd);
    return 0;
}
