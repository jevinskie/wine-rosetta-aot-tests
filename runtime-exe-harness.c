#include <stdio.h>
#include <mach/ndr.h>

int main(void) {
    printf("&fopen: %p\n", &fopen);
    printf("&NDR_record: %p\n", &NDR_record);
    printf("&NDR_record.reserved1: %p\n", &NDR_record.reserved1);
    printf("NDR_record.reserved1 before: %d\n", NDR_record.reserved1);
    NDR_record.reserved1 = 7;
    printf("NDR_record.reserved1 after: %d\n", NDR_record.reserved1);
    return 0;
}
