#include "fact-common.h"

u64 fact(u64 n);

u64 fact(u64 n) {
    u64 res = 1;
    if (n > 21) {
        return U64_MAX;
    }
    while (n > 1) {
        res *= n--;
    }
    return res;
}
