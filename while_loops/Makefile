all: while_loops

while_loops: while_loops.c while_loops.h
	gcc  while_loops.c -o while_loops -lpthread -Wall -Wextra -Werror -pedantic -g -fsanitize=thread

run:
	sh run.sh
	
clean:
	rm -f while_loops