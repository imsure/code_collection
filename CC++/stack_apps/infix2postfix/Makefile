SRCS_RAW = $(wildcard *.c)        
SRCS = $(filter-out balance_symbols.c, $(SRCS_RAW))
OBJS = $(SRCS:.c = .o)
#OBJS_TARGET = $(filter-out balance_symbols.o, $(OBJS))
CC = gcc 
#CCFLAGS = -g -Wall -O0 
CCFLAGS = -g -O0 

infix2postfix : $(OBJS)
	    $(CC) $^ -o $@  

%.o : %.c 
	    $(CC) -c $< $(CCFLAGS) 

clean:
	    \rm *.o
