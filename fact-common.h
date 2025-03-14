#pragma once

#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b)  ASSERT_CONCAT_(a, b)
/* These can't be used after statements in c89. */
#ifdef __COUNTER__
#define STATIC_ASSERT(e, m)                                           \
    enum {                                                            \
        ASSERT_CONCAT(static_assert_, __COUNTER__) = 1 / (int)(!!(e)) \
    }
#else
/* This can't be used twice on the same line so ensure if using in headers
 * that the headers are not included twice (by wrapping in #ifndef...#endif)
 * Note it doesn't cause an issue when used on same line of separate modules
 * compiled with gcc -combine -fwhole-program.  */
#define STATIC_ASSERT(e, m)                                      \
    enum {                                                       \
        ASSERT_CONCAT(assert_line_, __LINE__) = 1 / (int)(!!(e)) \
    }
#endif

typedef unsigned int u32;
typedef int i32;
typedef long int i64;
typedef unsigned long int u64;

STATIC_ASSERT(sizeof(u32) == 4, "u32 is not 4 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 is not 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 is not 8 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 is not 4 bytes");

#define U32_MAX 0xFFFFFFFFu
#define I32_MAX 0x7FFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFFul
#define I64_MAX 0x7FFFFFFFFFFFFFFFl
