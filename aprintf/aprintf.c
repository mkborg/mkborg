// The author disclaims copyright to this source code.
// This work is in public domain and is free of copyright restrictions.
// Everyone is free to use it in any non-harmfull way.

#include "aprintf.h"

#include <stddef.h> // NULL, size_t
#include <stdio.h> // vsnprintf()
#include <stdlib.h> // free(), malloc()

char * aprintf(const char * format, ...)
{
    char * s;
    va_list args;

    va_start(args, format);
    s = vaprintf(format, args);
    va_end(args);

    return s;
}

// Note: On buffer overflow some ancient "vsnprintf" implementations may
// return -1 instead of size necessary for complete output.
#define VAPRINTF_INITIAL_BUFSIZE 0x1000
char * vaprintf(const char * format, va_list args)
{
    char * s;
    size_t buf_size; // including trailing '\0'

    // Let's determine size of buffer necessary to consume complete output.
    // To speed-up below loop let's start with big enough buffer.
    buf_size = VAPRINTF_INITIAL_BUFSIZE;
    for(;;) {
	va_list args_copy;
	int result;

	if (!(s = malloc(buf_size))) { return s; } // out of memory

	va_copy(args_copy, args);
	result = vsnprintf(s, buf_size, format, args_copy);
	va_end(args_copy);
	free(s);

	if (result < 0) { // buffer overflow report from old glibc
	    buf_size *= 2; continue; // try again with bigger buffer
	}

	// "result" doesn't include trailing '\0'
	buf_size = result + 1; // one extra byte for trailing '\0'
	break;
    }

    // allocate necessary storage
    if (!(s = malloc(buf_size))) { return s; } // out of memory

    vsnprintf(s, buf_size, format, args); // must be successfull

    return s;
}
