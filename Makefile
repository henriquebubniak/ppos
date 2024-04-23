CC = gcc
CFLAGS = -Wall -g
SRCS = pingpong-tasks3.c ppos_core.c
OBJS = $(SRCS:.c=.o)
TARGET = main

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)

