
all: hello

hello: hello.c
	mpicc -o hello hello.c

run:
	mpirun --hostfile remotes.txt -np 92 ./hello

clean:
	rm -f hello