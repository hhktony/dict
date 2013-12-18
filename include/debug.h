#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#ifdef __NODEBUG__
#define IDEBUG(M, ...)
#else
#define IDEBUG(M, ...) \
    fprintf(stderr,\
        "[\033[33mDEBUG\033\[m] (%s:%d: ) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define IERROR(M, ...) do {\
    int errno_save = errno; \
    fprintf(stderr,\
        "[\033[31mERROR\033\[m] (%s:%d: errno: %s) " M "\n",\
        __FILE__,\
        __LINE__,\
        clean_errno(),\
        ##__VA_ARGS__\
    ); \
    exit(errno_save); \
} while (0)
#endif /* end of include guard: _DEBUG_H_ */
