
#ifndef FAB_GE_H
#define FAB_GE_H

#include <errno.h>

#define FAB_GE()                \
    do                          \
        goto error;             \
    while(0)

#define FAB_CGE(cond)           \
    do                          \
        if (cond)               \
            goto error;         \
    while(0)

#define FAB_GE_ERRNO(newerrno)              \
    do {                                    \
        errno = newerrno;                   \
        goto error;                         \
    } while(0)

#define FAB_CGE_ERRNO(cond, newerrno)       \
    do {                                    \
        if (cond) {                         \
            errno = newerrno;               \
            goto error;                     \
        }                                   \
    } while(0)

#define FAB_CGE_NULL(expr) FAB_CGE((expr) == NULL)
#define FAB_CGE_NEG(expr) FAB_CGE((expr) < 0)

#endif /* FAB_GE_H */
