# Compiler
CC = gcc

# Compiler flags: -I for include paths, -W for warnings
# -Wall is added as it's good practice to enable all common warnings
CFLAGS = -Iinclude -Wno-unsequenced -Wall

# Directories
ODIR = o

# Default target that runs when you just type "make"
all: $(ODIR)/map.o

# Rule to build the object file from the source file
# $@ is an automatic variable for the target name (o/map.o)
# $< is an automatic variable for the first dependency (src/map.c)
$(ODIR)/map.o: src/map.c
	@mkdir -p $(ODIR) # Create the output directory if it doesn't exist
	$(CC) -c $< -o $@ $(CFLAGS)

# Rule to clean up generated files
clean:
	rm -f $(ODIR)/*.o

# Tells make that "all" and "clean" are not actual files
.PHONY: all clean
