#include <stdio.h>
#include <mach/ndr.h>

extern void dylibify_entry(void);

int main(void) {
    printf("&fopen: %p\n", &fopen);
    printf("&dylibify_entry: %p\n", &dylibify_entry);
    printf("&NDR_record: %p\n", &NDR_record);
    printf("&NDR_record.reserved1: %p\n", &NDR_record.reserved1);
    printf("NDR_record.reserved1 before: %d\n", NDR_record.reserved1);
    NDR_record.reserved1 = 7;
    printf("NDR_record.reserved1 after: %d\n", NDR_record.reserved1);
    printf("calling dylibify_entry bye bye world\n");
    fflush(stdout);
    dylibify_entry();
    return 0;
}
