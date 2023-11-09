INCLDIR=include
SRCDIR=src
BUILDDIR=build
CC=gcc
CFLAGS=-I$(INCLDIR)

_DEPS = graph.h platform_specific.h error_handling.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJ = main.o graph.o 
OBJ = $(patsubst %,$(BUILDDIR)/%,$(_OBJ))


$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@mkdir -p $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILDDIR)/bajo-taio: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
