#include "std_string_printf.h"
#include <cstdio>

int main()
{
    {
	std::string s = std_string_printf("");
	fprintf(stdout, "size=%lu capacity=%lu s='%s'\n", s.size(), s.capacity(), s.c_str());
    }
    {
	std::string s = std_string_printf("test");
	fprintf(stdout, "size=%lu capacity=%lu s='%s'\n", s.size(), s.capacity(), s.c_str());
    }
}
