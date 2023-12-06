all: count_threes

count_threes: count_threes.c
	/usr/lib64/mpi/gcc/openmpi4/bin/mpicc -o count_threes count_threes.c

run:
	/usr/lib64/mpi/gcc/openmpi4/bin/mpirun --hostfile remotes.txt -np 92 ./count_threes 10000000

clean:
	rm -f count_threes
