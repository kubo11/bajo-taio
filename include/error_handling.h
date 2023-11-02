#ifndef BAJO_TAIO_ERROR_HANDLING_H
#define BAJO_TAIO_ERROR_HANDLING_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform_specific.h" 

#define ASSERT(x, msg) do {                                                                             \
  int retval = (x);                                                                                     \
  if (retval == 0) {                                                                                    \
    fprintf(stderr, "%s"ENDLINE, msg);                                                                  \
    fprintf(stderr, "Runtime error at %s:%d"ENDLINE, __FILE__, __LINE__);                               \
    exit(EXIT_FAILURE);                                                                                 \
  }                                                                                                     \
} while (0)

#define CHECK(x) do {                                                                                   \
  int retval = (x);                                                                                     \
  if (retval != 0) {                                                                                    \
    fprintf(stderr, "Runtime error: %s returned %d at %s:%d"ENDLINE, #x, retval, __FILE__, __LINE__);   \
    fprintf(stderr, "%s"ENDLINE, strerror(errno));                                                      \
    exit(EXIT_FAILURE);                                                                                 \
  }                                                                                                     \
} while (0)

#endif // BAJO_TAIO_ERROR_HANDLING_H