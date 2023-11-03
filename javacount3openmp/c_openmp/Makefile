
all: count_threes

count_threes: count_threes.c
	gcc -fopenmp -o count_threes count_threes.c

run:
	# run with 1 thread
	export OMP_NUM_THREADS=1
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "1 thread: " s/5}' results.txt

	# run with 2 threads
	export OMP_NUM_THREADS=2
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "2 threads: " s/5}' results.txt

	# run with 4 threads
	export OMP_NUM_THREADS=4
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "4 threads: " s/5}' results.txt

	# run with 8 threads
	export OMP_NUM_THREADS=8
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "8 threads: " s/5}' results.txt

	# run with 16 threads
	export OMP_NUM_THREADS=16
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "16 threads: " s/5}' results.txt

	# run with 32 threads
	export OMP_NUM_THREADS=32
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	./count_threes 10000000 >> results.txt
	awk '{s+=$$1} END {print "32 threads: " s/5}' results.txt

clean:
	rm -f count_threes results.txt

