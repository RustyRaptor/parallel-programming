all: witharray

witharray: witharray.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o witharray witharray.c

clean:
	rm -rf witharray
