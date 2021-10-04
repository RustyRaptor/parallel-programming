all: witharray_shm

witharray_shm: witharray_shm.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -o witharray_shm witharray_shm.c

clean:
	rm -rf witharray_shm
