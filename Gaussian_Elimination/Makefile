all: gaussian_elimination

gaussian_elimination: gaussian_elimination.c
	gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o gaussian_elimination gaussian_elimination.c

clean:
	rm -rf gaussian_elimination
