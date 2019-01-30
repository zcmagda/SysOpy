CC = gcc
CFLAGS = -Wall
PROGNAME = task1

all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).c
	 $(CC) $(CFLAGS) -o $(PROGNAME) $(PROGNAME).c

.PHONY: clean

clean:
	-rm -rf *.o $(PROGNAME)

test: $(PROGNAME)
	./$(PROGNAME) generate plik1 100 512
	./$(PROGNAME) generate plik2 100 512
	./$(PROGNAME) copy plik1 plik2 100 512 sys
	./$(PROGNAME) generate dane1 100 512
	./$(PROGNAME) generate dane2 100 512
	./$(PROGNAME) copy dane1 dane2 100 512 lib
