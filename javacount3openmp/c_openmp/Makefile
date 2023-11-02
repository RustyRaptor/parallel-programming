all: count_threes

count_threes: count_threes.c
	gcc -fopenmp -o count_threes count_threes.c

run:
	export OMP_NUM_THREADS=30
	./count_threes

clean:
	rm -f count_threes