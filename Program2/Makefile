all: sharedmem

sharedmem: sharedmem.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o sharedmem sharedmem.c

clean:
	rm -rf sharedmem
