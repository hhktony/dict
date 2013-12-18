CC = gcc
CPPFLAGS = -I../include
CFLAGS = -Wall -g -D__NDEBUG__
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

dict : $(OBJS)
	$(CC) $^ -o $@

%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJS) temp.txt dict *~

.PHONY : clean all
