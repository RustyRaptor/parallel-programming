
all: count_threes

count_threes: count_threes.c
	gcc -fopenmp -o count_threes count_threes.c

run:
	for threads in 1 2 4 8 16 32 64; do \
		echo "$$threads threads" >> results.txt; \
		for i in {1..5}; do \
			OMP_NUM_THREADS=$$threads ./count_threes 10000 >> results.txt; \
		done; \
	done

	
clean:
	rm -f count_threes results.txt

