CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=199309L

all: array_max prefix_sum_O0 prefix_sum_O2 prefix_sum_O3 matmul mergesort bfs

array_max: src/array_max.c
	$(CC) $(CFLAGS) -O3 -o array_max src/array_max.c

prefix_sum_O0: src/prefix_sum.c
	$(CC) $(CFLAGS) -O0 -o prefix_sum_O0 src/prefix_sum.c

prefix_sum_O2: src/prefix_sum.c
	$(CC) $(CFLAGS) -O2 -o prefix_sum_O2 src/prefix_sum.c

prefix_sum_O3: src/prefix_sum.c
	$(CC) $(CFLAGS) -O3 -o prefix_sum_O3 src/prefix_sum.c

matmul: src/matmul.c
	$(CC) $(CFLAGS) -O3 -o matmul src/matmul.c

mergesort: src/merge_sort.c
	$(CC) $(CFLAGS) -O3 -o mergesort src/merge_sort.c

bfs: src/bfs.c
	$(CC) $(CFLAGS) -O3 -o bfs src/bfs.c

clean:
	rm -f array_max prefix_sum_O0 prefix_sum_O2 prefix_sum_O3 matmul mergesort bfs

.PHONY: all clean
