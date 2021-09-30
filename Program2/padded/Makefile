all: padded

padded: padded.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o padded padded.c

clean:
	rm -rf padded
