# Questions

## 1. Generate random addresses with the following arguments

Generate random addresses with the following arguments:

- `-s 0 -n 10`
- `-s 1 -n 10`
- `-s 2 -n 10`

Change the policy from FIFO, to LRU, to OPT. Compute whether each access in said address traces are hits or misses.


# 1. `-s 0 -n 10`

## FIFO

    Access: 8  MISS -> [8]  [Hits:0 Misses:1]
    Access: 7  MISS -> [8,7] [Hits:0 Misses:2]
    Access: 4  MISS -> [8,7,4] [Hits:0 Misses:3]
    Access: 2  MISS -> [7,4,2] Replace 8 [Hits:0 Misses:4]
    Access: 5  MISS -> [4,2,5] Replace 7 [Hits:0 Misses:5]
    Access: 4  HIT -> [4,2,5] [Hits:1 Misses:4]
    Access: 7  MISS -> [2,5,7] Replace 4 [Hits:1 Misses:6]
    Access: 3  MISS -> [5,7,3] Replace 2 [Hits:1 Misses:7]
    Access: 4  MISS -> [7,3,4] Replace 5 [Hits:1 Misses:8]
    Access: 5  MISS -> [3,4,5] Replace 7 [Hits:1 Misses:9]


## LRU

    Access: 8  MISS -> [8] [Hits:0 Misses:1]
    Access: 7  MISS -> [8,7] [Hits:0 Misses:2]
    Access: 4  MISS -> [8,7,4] [Hits:0 Misses:3]
    Access: 2  MISS -> [7,4,2] Replace 8 [Hits:0 Misses:4]
    Access: 5  MISS -> [4,2,5] Replace 7 [Hits:0 Misses:5]
    Access: 4  HIT -> [2,5,4] [Hits:1 Misses:4]
    Access: 7  MISS -> [5,4,7] Replace 2 [Hits:1 Misses:6]
    Access: 3  MISS -> [4,7,3] Replace 5 [Hits:1 Misses:7]
    Access: 4  HIT -> [7,3,4] [Hits:2 Misses:7]
    Access: 5  MISS -> [3,4,5] Replace 3 [Hits:2 Misses:8]


## OPT

    Access: 8 MISS -> [8] [Hits:0 Misses:1]
    Access: 7 MISS -> [8,7] [Hits:0 Misses:2]
    Access: 4 MISS -> [8,7,4] [Hits:0 Misses:3]
    Access: 2 MISS -> [2,7,4] [Hits:0 Misses:4]
    Access: 5 MISS -> [5,7,4] [Hits:0 Misses:5]
    Access: 4 HIT -> [5,7,4] [Hits:1 Misses:5]
    Access: 7 HIT -> [5,7,4] [Hits:2 Misses:5]
    Access: 3 MISS -> [5,3,4] [Hits:2 Misses:6]
    Access: 4 HIT -> [5,3,4] [Hits:3 Misses:6]
    Access: 5 HIT -> [5,3,4] [Hits:4 Misses:6]


# 1. `-s 1 -n 10`

## FIFO

    Access: 1 MISS [1]
    Access: 8 MISS [1,8]
    Access: 7 MISS [1,8,7]
    Access: 2 MISS [8,7,2] rep:1
    Access: 4 MISS [7,2,4] rep:8
    Access: 4 HIT [7,2,4]
    Access: 6 MISS [2,4,6] rep:7
    Access: 7 MISS [4,6,7] rep:2
    Access: 0 MISS [6,7,0] rep:4
    Access: 0 HIT [6,7,0]

    Hits:2 Misses:8


## LRU

    Access: 1 MISS [1]
    Access: 8 MISS [1,8]
    Access: 7 MISS [1,8,7]
    Access: 2 MISS [8,7,2] rep:1
    Access: 4 MISS [7,2,4] rep:8
    Access: 4 HIT [7,2,4]
    Access: 6 MISS [2,4,6] rep:7
    Access: 7 MISS [4,6,7] rep:2
    Access: 0 MISS [6,7,0] rep:4
    Access: 0 HIT [6,7,0]

    Hits:2 Misses:8


## OPT

    Access: 1 MISS [1]
    Access: 8 MISS [1,8]
    Access: 7 MISS [1,8,7]
    Access: 2 MISS [8,7,2] rep:1
    Access: 4 MISS [7,2,4] rep:8
    Access: 4 HIT [7,2,4]
    Access: 6 MISS [7,4,6] rep:2
    Access: 7 HIT [7,4,6]
    Access: 0 MISS [6,7,0] rep:4
    Access: 0 HIT [6,7,0]

    Hits:3 Misses:7


# 1. `-s 2 -n 10`

## FIFO

    Access: 9 MISS [9]
    Access: 9 HIT [9]
    Access: 0 MISS [9,0]
    Access: 0 HIT [9,0]
    Access: 8 MISS [9,0,8]
    Access: 7 MISS [0,8,7] rep 9
    Access: 6 MISS [8,7,6] rep 0
    Access: 3 MISS [7,6,3] rep 8
    Access: 6 HIT [7,6,3]
    Access: 6 HIT [7,6,3]

    Hits:4 Misses:6


## LRU

    Access: 9 MISS [9]
    Access: 9 HIT [9]
    Access: 0 MISS [9,0]
    Access: 0 HIT [9,0]
    Access: 8 MISS [9,0,8]
    Access: 7 MISS [0,8,7] rep 9
    Access: 6 MISS [8,7,6] rep 0
    Access: 3 MISS [7,6,3] rep 8
    Access: 6 HIT [7,3,6]
    Access: 6 HIT [7,3,6]

    Hits:4 Misses:6


## OPT

    Access: 9 MISS [9]
    Access: 9 HIT [9]
    Access: 0 MISS [9,0]
    Access: 0 HIT [9,0]
    Access: 8 MISS [9,0,8]
    Access: 7 MISS [9,0,7] rep:8
    Access: 6 MISS [9,0,6] rep:7
    Access: 3 MISS [9,6,3] rep:0
    Access: 6 HIT [9,6,3]
    Access: 6 HIT [9,6,3]

    Hits:4 Misses:6


# 2. Worst-case address reference streams

For a cache of size 5, generate worst-case address reference streams for FIFO, LRU, and MRU.

## FIFO

    1 2 3 4 5 6 1 2 3 4 5 6 ...


## LRU

    1 2 3 4 5 6 1 2 3 4 5 6 ...


## MRU

    1 2 3 4 5 6 5 6 5 6 5 6 ...


If you would add only a single element more to the cache, performance would improve drastically and approach OPT.


# 3. Random trace

Generate a random trace using Python:

    python generate_trace.py


Trace output:

    3,2,8,7,6,0,3,5,4


Results:

    FIFO: Hits:0
    LRU: Hits:0
    OPT: Hits:2


# 4. Trace with locality

80-20 Workload:

    python generate_trace.py -l


Example output:

    0,1,0,1,8,0,1,1,0,0,1,1,0,1,7,1,1,0,5,0,1,6,1,1,0,0,1,0,1,0,2,1,1,1,4,1,0,1,0,0,0,0,1,1,0,1,1,1,1,0


The difference of LRU/RAND performance depends a lot on how long the trace is.

CLOCK:

The more clock bits, the more accurate the history displayed through the clock bits becomes, so the accuracy increases in this case.


# 5. Real application trace using Valgrind

Use:

    valgrind --tool=lackey --trace-mem=yes ls


The generated addresses are transformed into virtual page numbers by masking off the offset and shifting the resulting bits downward.


## FIFO

    Cache Size = 1 -> 48%
    Cache Size = 2 -> 79%
    Cache Size = 3 -> 89%
    Cache Size = 4 -> 90%
    Cache Size = 5 -> 92%
    Cache Size = 6 -> 93.5%


A cache size of 3 is fitting to satisfy a large fraction of requests.