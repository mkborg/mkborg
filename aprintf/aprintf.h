#if !defined aprintf_h
#define aprintf_h

// The author disclaims copyright to this source code.
// This work is in public domain and is free of copyright restrictions.
// Everyone is free to use it in any non-harmfull way.

#include <stdarg.h> // va_list

#if defined __cplusplus
extern "C" {
#endif

/*
    The "asprintf()" and "vasprintf()" functions are analogs of "sprintf()" and
    "vsprintf()", except that they allocate a string large enough to hold the
    output including the terminating null byte, and return a pointer to it.
    This pointer should be passed to "free()" to release the allocated storage
    when it is no longer needed. If memory allocation wasn't possible these
    functions return NULL.

    Note: In case of success output can't be truncated and always complete.
    Actual size of output can be obtained with "strlen()" if necessary.

    The "aprintf()" and "vaprintf()" functions are very similar to "asprintf()"
    and "vasprintf()" respectively. The only difference is in the way used to
    return the allocated string.
*/
char * aprintf(const char * format, ...);
char * vaprintf(const char * format, va_list args);

#if defined __cplusplus
}
#endif

#endif // !defined aprintf_h
