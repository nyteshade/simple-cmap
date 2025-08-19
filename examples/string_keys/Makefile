# Compiler
CC = gcc

# Compiler flags: -I for include paths, -W for warnings
# -Wall is added as it's good practice to enable all common warnings
CFLAGS = -I../../include -o main ../../o/map.o

# Directories
ODIR = .

# Default target that runs when you just type "make"
all: $(ODIR)/main

# Rule to build the object file from the source file
# $@ is an automatic variable for the target name (o/map.o)
# $< is an automatic variable for the first dependency (src/map.c)
$(ODIR)/main: main.c
	$(CC) main.c $(CFLAGS)

# Rule to clean up generated files
clean:
	rm -f $(ODIR)/main

# Tells make that "all" and "clean" are not actual files
.PHONY: all clean
