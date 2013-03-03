# MakeFile

# Target output file
LINK_TARGET = shell

# Objects - ADD ALL NEW .c FILES HERE AS .o
OBJS = prompt.o shell.o fileGet.o

# Headers
HEADERS = definitions.h fileGet.h prompt.h

# Compiler
CC = gcc

# Flags
CFLAGS = -g -Wall

# All Rebuildable files (objects and binaries)
REBUILDABLES = ${OBJS} ${LINK_TARGET}

all : ${LINK_TARGET}
	@echo All Done

# Link all object files and create executable
$(LINK_TARGET) : $(OBJS) 
	$(CC) -g -o $@ $^

# Create individual object files for every .c file
%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<
	@echo -n "\n" # using echo in make acts strange...

# Remove all object files and output file
clean:
	rm ${REBUILDABLES}
	@echo Clean Done
