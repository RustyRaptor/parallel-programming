all: testthread

testthread: testthread.c
	gcc -pthread -o testthread testthread.c

clean:
	rm -rf testthread
	
