set INCLDIR=include
set SRCDIR=src
set BUILDDIR=build
set CC=gcc
set CFLAGS=-I%INCLDIR% -g -Wall -lm
set DEPS=include/graph.h include/platform_specific.h include/error_handling.h include/maximum_common_subgraph.h include/bitset.h include/helpers.h
set OBJ=build/main.o build/graph.o build/maximum_common_subgraph.o build/bitset.o build/helpers.o

if not exist %BUILDDIR% mkdir %BUILDDIR%

%CC% -c -o %BUILDDIR%\main.o %SRCDIR%\main.c %CFLAGS%
%CC% -c -o %BUILDDIR%\graph.o %SRCDIR%\graph.c %CFLAGS%
%CC% -c -o %BUILDDIR%\bitset.o %SRCDIR%\bitset.c %CFLAGS%
%CC% -c -o %BUILDDIR%\maximum_common_subgraph.o %SRCDIR%\maximum_common_subgraph.c %CFLAGS%
%CC% -c -o %BUILDDIR%\helpers.o %SRCDIR%\helpers.c %CFLAGS%

%CC% -o %BUILDDIR%\bajo-taio.exe %OBJ% %DEPS% %CFLAGS%