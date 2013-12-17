CC = gcc
CFLAGS = -Wall -g
SRCS = $(wildcard *.c)
TARGETS = $(patsubst %.c, %, $(SRCS))

all : $(TARGETS)

$(TARGETS) : % : %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGETS) temp.txt

.PHONY : clean all
