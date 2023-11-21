#!/bin/bash

INCLDIR=include
SRCDIR=src
BUILDDIR=build
CC=gcc
CFLAGS="-I"$INCLDIR" -g -Wall -lm"

_DEPS="graph.h platform_specific.h error_handling.h maximum_common_subgraph.h bitset.h helpers.h"
_OBJ="main graph maximum_common_subgraph bitset helpers"

mkdir -p $BUILDDIR

for D in $_DEPS
do
    DEPS=$INCLDIR/$D" "$DEPS
done

for O in $_OBJ
do
    OBJ=$BUILDDIR/$O.o" "$OBJ
    $CC -c -o $BUILDDIR/$O.o $SRCDIR/$O.c $CFLAGS
done

$CC -o $BUILDDIR/bajo-taio $OBJ $DEPS $CFLAGS
