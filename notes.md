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