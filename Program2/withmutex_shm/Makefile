all: withmutex_shm

withmutex_shm: withmutex_shm.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -o withmutex_shm withmutex_shm.c

clean:
	rm -rf withmutex_shm
