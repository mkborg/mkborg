// The author disclaims copyright to this source code.
// This work is in public domain and is free of copyright restrictions.
// Everyone is free to use it in any non-harmfull way.

#include "std_string_printf.h"

#include <cstdarg> // ::va_list
#include <cstdio> // ::vsnprintf()
#include <string> // std::string

/*
    The "string_printf()" and "string_vprintf()" functions are analogs of
    "sprintf()" and "vsprintf()" (or "asprintf()" and "vasprintf()"),
    except that they return std::string.

    Note: Output is never truncated and always complete. Actual size of
    output can be obtained with "size()" or "length()" if necessary.
*/

std::string
std_string_printf(const char * format, ...)
{
    std::string s;
    va_list args;

    va_start(args, format);
    s = std_string_vprintf(format, args);
    va_end(args);

    return s;
}

// Note: On buffer overflow some ancient "vsnprintf" implementations may
// return -1 instead of size necessary for complete output.
#define STRING_VPRINTF_INITIAL_BUFSIZE 0x1000
std::string
std_string_vprintf(const char * format, va_list args)
{
    std::string s;
    size_t buf_size; // including trailing '\0'

    // Let's determine size of buffer necessary to consume complete output.
    // To speed-up below loop let's start with big enough buffer.
    buf_size = STRING_VPRINTF_INITIAL_BUFSIZE;
    for(;;) {
	va_list args_copy;
	int result;

	// Note: 'reserve()' doesn't change 'size()'
	s.reserve(buf_size);

	va_copy(args_copy, args);
	result = vsnprintf(&s[0], buf_size, format, args_copy);
	va_end(args_copy);

	if (result < 0) { // buffer overflow report from old glibc
	    buf_size *= 2; continue; // try again with bigger buffer
	}

	// "result" doesn't include trailing '\0'
	buf_size = result + 1; // one extra byte for trailing '\0'
	break;
    }

    // allocate necessary storage
    // Note: Initially 'size()' is zero and resize() fills string with '\0'
    s.resize(buf_size);

    vsnprintf(&s[0], buf_size, format, args); // must be successfull

    s.resize(buf_size - 1); // exclude trailing '\0'

    return s;
}
