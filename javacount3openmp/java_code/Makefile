all: CountThrees.class

CountThrees.class:
	javac CountThrees.java
run:	
	
	for threads in 1 2 4 8 16 32 64; do \
		echo "$$threads threads" >> results.txt; \
		for i in {1..5}; do \
			java CountThrees 1000000000 $$threads >> results.txt; \
		done \
	done


clean:
	rm -f CountThrees.class results.txt