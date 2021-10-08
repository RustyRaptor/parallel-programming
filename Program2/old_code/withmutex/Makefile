all: withmutex

withmutex: withmutex.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o withmutex withmutex.c

clean:
	rm -rf withmutex
