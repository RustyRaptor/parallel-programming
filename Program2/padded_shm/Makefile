all: padded_shm

padded_shm: padded_shm.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -o padded_shm padded_shm.c

clean:
	rm -rf padded_shm
