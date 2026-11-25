CC      := cc
CFLAGS  := -std=c11 -O2 -Wall -Wextra
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:.c=.o)

all: memprobe

memprobe: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c src/memprobe.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) memprobe

.PHONY: all clean
