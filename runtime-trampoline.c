#include "runtime-trampoline.h"

#include <mach-o/loader.h>

// Function prototype for special_func
extern void dylibify_entry(void);

// Trampoline function with CFI directives for proper unwinding
// and preservation of all registers
__attribute__((naked)) void runtime_trampoline(int dyld_fd, struct mach_header_64 *dyld_mh,
                                               int exe_fd, struct mach_header_64 *exe_mh, int argc,
                                               char *argv[]) {
    // In a naked function, parameters are in registers x0-x5 per ARM64 ABI
    asm volatile(
        // CFI setup
        ".cfi_startproc\n"

        // Setup frame with proper alignment
        // Save frame pointer (x29) and link register (x30)
        "stp fp, lr, [sp, #-16]!\n" // Pre-decrement SP by 16 bytes and save FP and LR
        ".cfi_def_cfa_offset 16\n"  // CFA is now SP+16
        ".cfi_offset fp, -16\n"     // fp saved at CFA-16
        ".cfi_offset lr, -8\n"      // lr saved at CFA-8

        "mov fp, sp\n"               // Update frame pointer
        ".cfi_def_cfa_register fp\n" // CFA is now based on FP

        // Save x9 to preserve its value - we'll need one more temp register (x10)
        "stp x9, x10, [sp, #-16]!\n" // Save x9 and x10 (for 16-byte alignment)
        ".cfi_offset x9, -16\n"      // x9 saved at CFA-16
        ".cfi_offset x10, -8\n"      // x10 saved at CFA-8

        // Allocate space for our custom call frame (48 bytes, rounded up to 64 for 16-byte
        // alignment)
        "sub sp, sp, #64\n"

        // Store the arguments at the specified offsets
        "str x0, [sp]\n"      // sp[0x00]: dyld_fd
        "str x1, [sp, #8]\n"  // sp[0x08]: dyld_mh
        "str x2, [sp, #16]\n" // sp[0x10]: exe_fd
        "str x3, [sp, #24]\n" // sp[0x18]: exe_mh
        "str x4, [sp, #32]\n" // sp[0x20]: argc
        "str x5, [sp, #40]\n" // sp[0x28]: argv

        // Load dylibify_entry address using x10 as our temporary
        "adrp x10, %0@GOTPAGE\n"
        "ldr x10, [x10, %0@GOTPAGEOFF]\n"

        // Call dylibify_entry
        "blr x10\n"

        // Clean up our custom stack frame
        "add sp, sp, #64\n"

        // Restore x9 and x10
        "ldp x9, x10, [sp], #16\n"

        // Reset CFA to use SP
        ".cfi_def_cfa_register sp\n" // CFA is now based on SP again

        // Restore frame pointer and link register
        "ldp fp, lr, [sp], #16\n"
        ".cfi_restore fp\n"
        ".cfi_restore lr\n"
        ".cfi_def_cfa_offset 0\n" // Reset CFA offset

        // Return to the original caller
        "ret\n"

        ".cfi_endproc\n"
        :
        : "i"(dylibify_entry)
        : "memory" // No register clobbers since we save and restore everything
    );
}
