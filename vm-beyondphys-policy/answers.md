Questions

1. Generate random addresses with the following arguments: -s 0
-n 10, -s 1 -n 10, and -s 2 -n 10. Change the policy from
FIFO, to LRU, to OPT. Compute whether each access in said address
traces are hits or misses.

1. -s 0 -n 10

FIFO:
Access: 8  MISS -> [8]  [Hits:0 Misses:1]
Access: 7  MISS -> [8,7] [Hits:0 Misses:2]
Access: 4  MISS ->[8,7,4] [Hits:0 Misses:3]
Access: 2  MISS -> [7,4,2] Replace 8[Hits:0 Misses:4]
Access: 5  MISS -> [4,2,5] Replace 7 [Hits:0 Misses:5]  
Access: 4  HIT -> [4,2,5]  [Hits:1 Misses:4]
Access: 7  MISS -> [2,5,7] Replace 4 [Hits:1 Misses:6]
Access: 3  MISS -> [5,7,3] Replace 2 [Hits:1 Misses:7]
Access: 4  MISS -> [7,3,4] Replace 5 [Hits:1 Misses:8]
Access: 5  MISS -> [3,4,5] Replace 7 [Hits:1 Misses:9]

LRU:
Access: 8  MISS -> [8]  [Hits:0 Misses:1]
Access: 7  MISS -> [8,7] [Hits:0 Misses:2]
Access: 4  MISS ->[8,7,4] [Hits:0 Misses:3]
Access: 2  MISS -> [7,4,2] Replace 8[Hits:0 Misses:4]
Access: 5  MISS -> [4,2,5] Replace 7 [Hits:0 Misses:5]  
Access: 4  HIT -> [2,5,4]  [Hits:1 Misses:4] 
Access: 7  MISS -> [5,4,7] Replace 2 [Hits:1 Misses:6]
Access: 3  MISS -> [4,7,3] Replace 5 [Hits:1 Misses7]
Access: 4  HIT -> [7,3,4] [Hits:2 Misses:7]
Access: 5  MISS -> [3,4,5] Replace 3 [Hits:2 Misses:8]

OPT:
Access: 8  MISS -> [8]  [Hits:0 Misses:1]
Access: 7  MISS -> [8,7] [Hits:0 Misses:2]
Access: 4  MISS ->[8,7,4] [Hits:0 Misses:3]
Access: 2  MISS ->[2,7,4] [Hits:0 Misses:4]
Access: 5  MISS ->[5,7,4] [Hits:0 Misses:5]
Access: 4 HIT -> [5,7,4] [Hits:1 Misses:5]
Access: 7 HIT -> [5,7,4] [Hits:2 Misses:5]
Access: 3 MISS -> [5,3,4] [Hits:2 Misses:6]
Access: 4  HIT [5,3,4] [Hits:3 Misses:6]
Access: 5  HIT [5,3,4] [Hits:4 Misses:6]


1. -s 1 -n 10
FIFO:
Access: 1  MISS [1]
Access: 8  MISS [1,8]
Access: 7  MISS [1,8,7]
Access: 2  MISS [8,7,2] rep:1
Access: 4  MISS [7,2,4] rep:8
Access: 4  HIT [7,2,4]
Access: 6  MISS [2,4,6] rep:7
Access: 7  MISS [4,6,7] rep:2
Access: 0  MISS [6,7,0] rep:4
Access: 0  HIT [6,7,0]

Hits:2 Misses:8

LRU:
Access: 1  MISS [1]
Access: 8  MISS [1,8]
Access: 7  MISS [1,8,7]
Access: 2  MISS [8,7,2] rep:1
Access: 4  MISS [7,2,4] rep:8
Access: 4  HIT [7,2,4]
Access: 6  MISS [2,4,6] rep:7
Access: 7  MISS [4,6,7] rep:2
Access: 0  MISS [6,7,0] rep:4
Access: 0  HIT [6,7,0]

Hits:2 Misses:8

OPT:
Access: 1  MISS [1]
Access: 8  MISS [1,8]
Access: 7  MISS [1,8,7]
Access: 2  MISS [8,7,2] rep:1
Access: 4  MISS [7,2,4] rep:8
Access: 4  HIT [7,2,4]
Access: 6  MISS [7,4,6] rep:2
Access: 7  HIT [7,4,6,] 
Access: 0  MISS [6,7,0] rep:4
Access: 0  HIT [6,7,0]

Hits:3 Misses:7

1. -s 2 -n 10

FIFO:
Access: 9  MISS [9]
Access: 9  HIT [9]
Access: 0  MISS [9,0]
Access: 0  HIT [9,0]
Access: 8  MISS [9,0,8] 
Access: 7  MISS [0,8,7] rep 9
Access: 6  MISS [8,7,6] rep 0
Access: 3  MISS [7,6,3] rep 8
Access: 6  HIT [7,6,3]
Access: 6  HIT [7,6,3]

Hits:4 Misses:6

LRU:
Access: 9  MISS [9]
Access: 9  HIT [9]
Access: 0  MISS [9,0]
Access: 0  HIT [9,0]
Access: 8  MISS [9,0,8]
Access: 7  MISS [0,8,7] rep 9
Access: 6  MISS [8,7,6] rep 0
Access: 3  MISS [7,6,3] rep 8
Access: 6  HIT [7,3,6]
Access: 6  HIT [7,3,6]

Hits:4 Misses:6

OPT:
Access: 9  MISS [9]
Access: 9  HIT [9]
Access: 0  MISS [9,0]
Access: 0  HIT [9,0]
Access: 8  MISS [9,0,8]
Access: 7  MISS [9,0,7] rep 8
Access: 6  MISS [9,0,6] rep 7
Access: 3  MISS [9,6,3] rep 0
Access: 6  HIT [9,6,3]
Access: 6  HIT [9,6,3]

Hits:4 Misses:6

2. For a cache of size 5, generate worst-case address reference streams
for each of the following policies: FIFO, LRU, and MRU (worst-case
reference streams cause the most misses possible. For the worst case
reference streams, how much bigger of a cache is needed to improve
performance dramatically and approach OPT?


FIFO: 1 2 3 4 5 6 1 2 3 4 5 6 etc.

LRU: 1 2 3 4 5 6 1 2 3 4 5 6 etc.

MRU: 1 2 3 4 5 6 5 6 5 6 5 6 etc.

If you would add only a single element more to the cache, performance would improve drastically. 

3. Generate a random trace (i.e., use python and write a script that
outputs random addresses, which you can then feed into the simulator). How would you expect the different policies to perform on
such a trace?

4. Now generate a trace with some locality. How can you generate
such a trace? How does LRU perform on it? How much better than
RAND is LRU? How does CLOCK do? How about CLOCK with
different numbers of clock bits?

5. Use a program like valgrind to instrument a real application and
generate a virtual page reference stream. For example, running
valgrind --tool=lackey --trace-mem=yes ls will output
a nearly-complete reference trace of every instruction and data reference made by the program ls. To make this useful for the simulator above, you’ll have to first transform each virtual memory
reference into a virtual page-number reference (done by masking
off the offset and shifting the resulting bits downward). How big
of a cache is needed for your application trace in order to satisfy a
large fraction of requests? Plot a graph of its working set as the size
of the cache increases.
