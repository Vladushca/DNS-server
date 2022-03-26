CC = gcc

CFLAGS = -Wall

SRCS = tema1.c functii_hash.c functii_list.c

OBJS = $(SRCS:.c=.o)

MAIN = tema1

.PHONY: clean

all: build

run: build
	./$(MAIN)

build: $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	
.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@
	
clean:
	$(RM) ./*.o *~ $(MAIN)
