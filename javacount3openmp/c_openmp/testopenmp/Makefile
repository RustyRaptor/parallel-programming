all: testopenmp run

testopenmp:
	gcc -fopenmp -o testopenmp testopenmp.c

run:
	export OMP_NUM_THREADS=30
	./testopenmp

clean:
	rm -f testopenmp