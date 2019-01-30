CC = gcc
CFLAGS = -Wall -std=c11 -ggdb
PROGNAME = filesInfo

default: all

all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).c
	 $(CC) $(CFLAGS) -o $(PROGNAME) $(PROGNAME).c

.PHONY: clean

clean:
	-rm -rf *.o $(PROGNAME)

test: $(PROGNAME)
	./$(PROGNAME) ~/Documents/SystemyOperacyjne2018/ZajacMagdalena '>' '2018-12-01 00:00:00'
	./$(PROGNAME) ~/Documents/SystemyOperacyjne2018/ZajacMagdalena '<' '2018-12-01 00:00:00'
	./$(PROGNAME) ~/Documents/SystemyOperacyjne2018/ZajacMagdalena '=' '2018-12-01 00:00:00'
	./$(PROGNAME) /home/madzia/Documents/SystemyOperacyjne2018/ZajacMagdalena '>' '2018-12-01 00:00:00'
