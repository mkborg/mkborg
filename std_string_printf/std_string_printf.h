#if !defined std_string_printf_h
#define std_string_printf_h

// The author disclaims copyright to this source code.
// This work is in public domain and is free of copyright restrictions.
// Everyone is free to use it in any non-harmfull way.

#include <cstdarg> // ::va_list
#include <string> // std::string

/*
    The "string_printf()" and "string_vprintf()" functions are analogs of
    "sprintf()" and "vsprintf()" (or "asprintf()" and "vasprintf()"),
    except that they return std::string.

    Note: Output can't be truncated and always complete. Actual size of
    output can be obtained with "size()" or "length()" if necessary.
*/

std::string std_string_printf(const char * format, ...);
std::string std_string_vprintf(const char * format, va_list args);

#endif // !defined std_string_printf_h
