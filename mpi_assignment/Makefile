all: hello

hello: hello.c
	/usr/lib64/mpi/gcc/openmpi4/bin/mpicc -o hello hello.c

run:
	/usr/lib64/mpi/gcc/openmpi4/bin/mpirun --hostfile remotes.txt -np 100 ./hello

clean:
	rm -f hello
