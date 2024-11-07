CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = ecosys 

.PHONY:	all clean

all: $(PROGRAMS)

ecosys: ecosys.o main_ecosys.o
	$(CC) -o $@ $(CFLAGS) $^

ecosys.o: ecosys.c
	gcc $(CFLAGS) -c ecosys.c

main_ecosys.o: main_ecosys.c 
	gcc $(CFLAGS) -c main_ecosys.c

clean:
	rm -f *.o *~ $(PROGRAMS)