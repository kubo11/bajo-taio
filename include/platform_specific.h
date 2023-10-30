#ifndef BAJO_TAIO_PLATFORM_SPECIFIC_H
#define BAJO_TAIO_PLATFORM_SPECIFIC_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# define ENDLINE "\r\n"
#else
# define ENDLINE "\n"
#endif // WINDOWS

#endif // BAJO_TAIO_PLATFORM_SPECIFIC_H