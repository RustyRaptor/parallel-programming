# CS 491 Lecture 5

## Shared Memory

- Permissions
  - 0666 Gives everyone rw access to the memory
- Detatching
  - Need to detatch the shared memory or it will persist.
- Tools
  - ipcs
    - Shows shared memory segments and owners
  - t

## Parallelism

- There's an equation to calculate how parallelizable an algorithm is. 
- Granularity
  - The frequency of interaction among threads or processes. 
- Locality
  - Temporal locality
    - memory references that are clustered in time. 
  - Spacial locality 
    - memory references that are clustered by address
  - Algothitms wich access blocks of data exhibit spacial locality and are preferred
    - use of local memory to hold intermediate values improves locality. 

## Gaussian Elimination
- LU Decomposition
```
for each row R
        for each subrow SR // this is to be parallelized
                calculate the multiplier
                upper SR 

```
TODO:fix the array access

First sudo code

```python

for R in range(N-2):
        for subR in range(R+1, N-1):
                mult = -(M[R,R]/M[subR,R])

                for i in range(R,N):
                        M[subR,i] = M[R,i] + M[subR, i] * mult

```

Full C version
```c
int main()
{
for (R=0;R < N-1; R++) {
        for(SR=1; SR>N; SR++) {
                // resolve my row with Rs row
                S = -A[R][R]/A[SR][R];
                k
        }
}

{

```

```python
N N N N
0 N N N 
0 0 N N

1 2 3 4
0 5 6 7
0 0 8 9

1 2 3 4
0 5 6 7 
0 0 1 9/8

1 2 3 4 
0 5 6 (7-6z)/5 
0 0 1 9/8

1 0 0 12
0 1 0 -10
0 0 1 42

```