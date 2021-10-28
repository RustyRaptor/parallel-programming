# Parallel Programming Midterm

### 1

##### Q
1) Provide a clear example of how the “localize()” function works in Peril-L and why it is an important concept in parallel programming. 

##### A

- `localize()` allows us to map the global memory to a local memory of a thread. 
- This is important because we need a standard way to talk about how we distribute and access the global data accross threads without going into detail of how that's being done in pseudocode.
- It also allows us to think about the section of global data allocated for a thread in a relative way.
  - Such that if we give a thread the last quarter of a global array we can access that section of the array starting at 0 rather than having to compute where we are in the global array.
- Here is an example of how we could use localize in Peril-L
  - This is a modified version of the example used in the book p.93.
  - This version will also demonstrate writing to the local reference.

```c
int _allData_ [n]; // This is a global array

forall( threadID in(0..P-l)) // let's spawn P threads
{
        int size=n/P; // The size of the section for this thread

        // This will map the appropriate section of
        // the _allData_ array based on size to the locData array. 
        int locData[size]=localize(_allData_[]); 

        // This will set all the values of our chunk of the array
        // to be equal to our threadID. Since this is mapped to 
        // the global array we can assume the global array will be
        // updated because the local and global are references to
        // the same location. 
        for (int i = 0; i < size; i++) {
                locData[i] = threadID;
        }
}
```

### 2

##### Q
2) Provide a description, detailed history, and performance characteristics of the Infiniband communications infrastructure. I am interested in when the subsystem becomes saturated, what is the normal connection topology, what systems support it, studies on how it performs both as a bench requirement as well as how well it performs with real applications. You need to have more than 3 references. 

##### A

- The Infiniband Trade Association started when two competing ideas, Future I/O and Next Generation I/O merged in 1999 [1]
- 


- References
  1.  https://www.eetimes.com/warring-ngio-and-future-i-o-groups-to-merge/
  2.  

### 3

##### Q
3) What are the differences between Speedup and 
Relative Speedup.  Convince me that you 
understand why we have the two terms.

##### A

- Speedup tells us the difference in performance between the Parallel computation using $P$ processors and the best serial computation $T_s$. 
- Relative Speedup on the other hand doesn't use the performance of the serial program $T_s$ but rather the performance of the parallel program with only one Process/Thread so it compares the parallel code when $P=1$ vs when we add more processors.
- The reason we have these separate terms is because the overhead of setting up a parallel program will mean that using 1 processor in a parallel version will be different from running it in serial sometimes. 
- Another reason is that Relative Speedup allows us to analyze how the parallel code scales up starting with one process. 

### 4

##### Q
4) Define and explain “efficiency” as it applies to parallel computing. Is it possible to have an efficiency greater than 1? If so, give examples. If not, state why not. 
##### A
- **Efficiency** is defined as the $speedup / P$ where $P$ is the number of processors used and $speedup$ is the improvement in performance from the serial algorithm to the parallel algorithm.
- Because speedup is based on the serial time divided by the parallel time we can only expect to get something higher than 1 if the serial time is significantly higher than the parallel time. 
- This can happen if the serial time is artificially inflated whether on accident or on purpose.
- It can also happen in rare cases like Program 1 in the book when all the memory needed by the processes can fit into the cache of the processors and so the parallel code has more efficient memory access.

### 5

##### Q



##### A