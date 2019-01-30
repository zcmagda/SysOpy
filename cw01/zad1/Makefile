CC = gcc
CFLAGS = -Wall

all: bubble fibo pfor rek

bubble: bubble.c
	$(CC) $(CFLAGS) -o bubble bubble.c

fibo: fibo.c
	$(CC) $(CFLAGS) -o fibo fibo.c

pfor: for.c
	$(CC) $(CFLAGS) -o pfor for.c

rek: rek.c
	$(CC) $(CFLAGS) -o rek rek.c

clean:
	-rm -rf *.o $(TARGET)

test:
	$(CC) $(CFLAGS) bubble.c -o bubble_test
	$(CC) $(CFLAGS) bubble.c -o bubble_test_o1  -O1
	$(CC) $(CFLAGS) bubble.c -o bubble_test_o2  -O2
	$(CC) $(CFLAGS) bubble.c -o bubble_test_o3  -O3
