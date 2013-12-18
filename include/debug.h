#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef __NDEBUG__
#define debug(M, ...)
#else
#define debug(M, ...) \
    fprintf(stderr,\
        "[\033[33mDEBUG\033\[m] (%s:%d: ) " M "\n",\
        __FILE__,\
        __LINE__,\
        ##__VA_ARGS__\
    )
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_err(M, ...) \
    fprintf(stderr,\
        "[\033[31mERROR\033\[m] (%s:%d: errno: %s) " M "\n",\
        __FILE__,\
        __LINE__,\
        clean_errno(),\
        ##__VA_ARGS__\
    )
#define log_warn(M, ...) \
    fprintf(stderr,\
        "[WARN] (%s:%d: errno: %s) " M "\n",\
        __FILE__,\
        __LINE__,\
        clean_errno(),\
        ##__VA_ARGS__\
    )
#define log_info(M, ...) \
    fprintf(stderr, \
        "[INFO] (%s:%d) " M "\n", \
        __FILE__, \
        __LINE__, \
        ##__VA_ARGS__\
    )

#endif /* end of include guard: _DEBUG_H_ */
