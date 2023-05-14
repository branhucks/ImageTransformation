CC 			= gcc
INCLUDE = -I.
CFLAGS 	= -g -Wall
LDFLAGS = -L. \
	-L/usr/lib
LDLIBS 	= \
	-lc -lm

.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c $^

all: main

main: main.o ppm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o
	rm -f main
