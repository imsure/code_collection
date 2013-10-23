CC = gcc
CFLAG = -Wall -O2 -std=gnu99
SRC = $(wildcard *.c)
OBJ = $(SRC:.c = .o)
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))

all: client

client: $(OBJ)
	$(CC) $(CFLAG) $? -o $@

%.o: %.c
	$(CC) $(CFLAG) -c $?

clean:
	\rm client