CC := gcc

SOURCE_DIR  := src

CFLAGS := -Wall -g -fPIC

CFILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJS   := $(patsubst $(SOURCE_DIR)/%.c, %.o, $(CFILES))

PROG_NAME := libsmart_pointers.so

$(PROG_NAME): $(OBJS)
	$(CC) -shared *.o -o $@ $(LFLAGS)

%.o: $(SOURCE_DIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf $(PROG_NAME) *.o
