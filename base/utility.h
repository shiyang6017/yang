#ifndef UTILITY_H
#define UTILITY_H

#include <assert.h>

namespace yang {
    /*used to check the return val in pthread_XXX function */
#define PCHECK(ret) do {                                                                     \
                            __typeof__ (ret) errnum = (ret);                                 \
                            if (__builtin_expect(errnum != 0, 0)) {                          \
                                __assert_perror_fail(errnum, __FILE__, __LINE__, __func__); \
                            }                                                                \
                       } while (0)

}

#endif