#ifndef BAJO_TAIO_PLATFORM_SPECIFIC_H
#define BAJO_TAIO_PLATFORM_SPECIFIC_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>

# define ENDLINE "\r\n"
# define TIME LARGE_INTEGER
# define MEASURE(t) QueryPerformanceCounter(&t)
# define TIME_SETUP() LARGE_INTEGER freq
# define DIFF(start, end, val) \
    QueryPerformanceFrequency(&freq); \
    val = (float) (end.QuadPart - start.QuadPart) / freq.QuadPart
#else
# include <time.h>

# define ENDLINE "\n"
# define TIME clock_t
# define MEASURE(t) t = clock()
# define TIME_SETUP() (void)0;
# define DIFF(start, end, val) val = (float)(end - start) / (float)CLOCKS_PER_SEC

#endif // WINDOWS

#endif // BAJO_TAIO_PLATFORM_SPECIFIC_H