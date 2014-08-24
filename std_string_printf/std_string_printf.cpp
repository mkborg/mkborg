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

// Minimal buffer must have space at least for terminating '\0'
#define MINIMAL_BUFSIZE 1

// Initial guess must cover frequent cases
#define INITIAL_GUESS_BUFSIZE 0x1000

std::string
std_string_vprintf(const char * format, va_list args)
{
    std::string s;
    va_list args_copy;
    size_t buf_size; // including trailing '\0'
    int result;

    // We need to determine size of buffer necessary to fit complete output.
    // Let's assume modern 'vsnprintf' always returning size necessary for
    // complete output. In such case we can use very small local buffer.
    {
	char small_buffer[MINIMAL_BUFSIZE];
	va_copy(args_copy, args);
	result = vsnprintf(small_buffer, sizeof(small_buffer), format, args_copy);
	va_end(args_copy);
    }
    if (result < 0) {
	// Buffer overflow report from old glibc.
	// We need real buffer big enough to fit complete output.
	// We have to guess and check iteratively.
	// To speed-up below loop let's start with reasonable big buffer.
	buf_size = INITIAL_GUESS_BUFSIZE;
	for(;;) {
	    // allocate storage
	    // Note: 'reserve()' doesn't change 'size()'
	    // Note: 'resize()' fills additional space with '\0' (initially
	    // 'size()' is zero).
	    // Note: 'reserve()' shall be faster than 'resize()' but final
	    // 'vsnprintf()' can be avoided if 'resize()' is used here.
	    s.reserve(buf_size);

	    va_copy(args_copy, args);
	    result = vsnprintf(&s[0], buf_size, format, args_copy);
	    va_end(args_copy);

	    if (result >= 0) { break; } // success

	    buf_size *= 2; // try again with bigger buffer

	    // FIXME: need to check for some reasonable upper limit
	}
    }
    // "result" doesn't include trailing '\0'
    buf_size = result + 1; // one extra byte for trailing '\0'

    // allocate necessary storage
    // Note: Initially 'size()' is zero and resize() fills string with '\0'
    s.resize(buf_size);

    vsnprintf(&s[0], buf_size, format, args); // must be successfull

    s.resize(buf_size - 1); // exclude trailing '\0'

    return s;
}
