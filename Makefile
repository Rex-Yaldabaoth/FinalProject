#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -Wextra -g -O2 -fopenmp

# define any directories containing header files other than /usr/include
INCLUDES =  -I ./inc/ -I /usr/local/include -I /usr/lib/

# define library paths
LFLAGS = 

# define any libraries to link into executable:
LIBS = -lm -lgsl -lgslcblas

# define the C source files
SOURCEDIR = ./src/
SRCS := $(shell find $(SOURCEDIR) -name '*.c')

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = ./bin/Dripping_Tap.exe

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

all:    $(MAIN)
#		@echo  Simple compiler named mycc has been compiled

$(MAIN): $(OBJS)
		rm ./bin/Dripping_Tap.exe || true
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
		find . -name "*.o" -type f -delete
		@printf "Done!\n" 

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it