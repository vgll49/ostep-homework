


**Question:** 1. We’ll start by redoing the measurements within this chapter. Use
the call gettimeofday() to measure time within your program.
How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the
cycle counter available on x86 via the rdtsc instruction.

**Answer:**

`gettimeofday()` returns the current time with microsecond precision (`struct timeval` contains seconds and microseconds).

I found that for measuring elapsed time, `CLOCK_MONOTONIC` is generally more suitable and switched to this instead. 



**Question:** 2. Now, build a simple concurrent counter and measure how long it
takes to increment the counter many times as the number of threads
increases. How many CPUs are available on the system you are
using? Does this number impact your measurements at all?

**Answer:**

My measurements show that adding more threads does not necessarily make a synchronized counter faster. In fact, the execution time increases with the number of threads.

| Threads | Execution time (µs) |
|---:|---:|
| 1 | 690 |
| 2 | 1,606 |
| 4 | 2,130 |
| 8 | 2,955 |

My system reports 12 logical CPUs according to `lscpu`.

Although multiple threads can potentially execute in parallel on different CPUs, the synchronized counter requires threads to acquire the same lock before modifying the counter. This creates lock contention.

As a result, threads spend time waiting for the lock instead of performing useful work. The synchronization overhead can outweigh the benefits of parallel execution.

---


**Question:** 3. Next, build a version of the approximate counter. Once again, measure its performance as the number of threads varies, as well as the
threshold. Do the numbers match what you see in the chapter?


**Answer:**

I implemented an approximate counter and measured its execution time with different numbers of threads and different thresholds.

The benchmark was performed with a target of 1,000,000 increments.

### Benchmark Results

| Threads | Threshold | Median time (µs) |
|---:|---:|---:|
| 1 | 100 | 7,647.20 |
| 1 | 1,000 | 7,458.19 |
| 1 | 10,000 | 7,650.91 |
| 2 | 100 | 4,486.99 |
| 2 | 1,000 | 4,104.13 |
| 2 | 10,000 | 3,959.57 |
| 4 | 100 | 3,734.56 |
| 4 | 1,000 | 3,455.55 |
| 4 | 10,000 | 3,698.40 |
| 8 | 100 | 3,163.31 |
| 8 | 1,000 | 2,137.96 |
| 8 | 10,000 | 2,073.91 |
| 12 | 100 | 3,060.65 |
| 12 | 1,000 | 2,368.34 |
| 12 | 10,000 | 2,511.07 |


The results show that the approximate counter generally performs better as the number of threads increases, up to eight threads.

This is different from the synchronized counter, where additional threads increased execution time because they frequently had to wait for the same lock.

The approximate counter reduces contention by allowing threads to update local counters and periodically synchronize their values with a global counter.

The threshold also has an impact on performance. Higher thresholds generally reduce the frequency of synchronization, which can improve performance.

In my measurements, a threshold of 1,000 or 10,000 performed better than a threshold of 100, particularly with eight threads.

At twelve threads, performance does not improve further. This could be caused by scheduling overhead, CPU resource limits, or remaining synchronization costs.

Overall, the results demonstrate the main idea of the approximate counter: reducing synchronization frequency can significantly improve scalability.

The results are consistent with the general behavior described in the chapter, although exact performance depends on the hardware, implementation, and workload.

---


**Question:** 4. Build a version of a linked list that uses hand-over-hand locking
[MS04], as cited in the chapter. You should read the paper first
to understand how it works, and then implement it. Measure its
performance. When does a hand-over-hand list work better than a
standard list as shown in the chapter?

**Answer:**

Hand-over-hand locking can theoretically outperform a standard linked list with a single global lock under high contention.

The main advantage is that multiple threads can operate on different parts of the list concurrently. Each thread acquires the lock of the next node before releasing the lock of the current node.

However, there is additional overhead from acquiring and releasing individual node locks. Furthermore, threads may still contend for locks near the beginning of the list, since traversals have to pass through those nodes.

Therefore, hand-over-hand locking is most useful when the list is sufficiently large, contention is high, and threads can work on different regions of the list.

---


**Question:** 5. Pick your favorite data structure, such as a B-tree or other slightly
more interesting structure. Implement it, and start with a simple
locking strategy such as a single lock. Measure its performance as
the number of concurrent threads increases.

**Answer:**

I chose to implement a Binary Search Tree (BST).

For the first implementation, I used a single global mutex to protect the entire tree. This means that only one thread can perform an insertion at a time.

The results show that execution time increases as the number of threads increases.

---


**Question:** 6. Finally, think of a more interesting locking strategy for this favorite
data structure of yours. Implement it, and measure its performance.
How does it compare to the straightforward locking approach?

**Answer:**

For the second implementation, I used node-based locking with lock coupling (inspired by the hand-over-hand locking).

Instead of protecting the entire tree with one mutex, each node has its own mutex. During traversal, a thread acquires the lock of the child node before releasing the lock of the parent node.

This theoretically allows multiple threads to work on different parts of the tree simultaneously.

However, threads still need to traverse shared upper nodes, especially the root and its children. This can create lock contention because multiple threads need to acquire the same locks before reaching different parts of the tree.

Additionally, node-based locking introduces more synchronization overhead because each insertion requires multiple lock and unlock operations.

### Benchmark Results

The following results show the median execution time for 1,000,000 insertions.

| Threads | Global Lock (ms) | Node-based Lock (ms) |
|---:|---:|---:|
| 1 | 499.00 | 859.21 |
| 2 | 1,093.29 | 554.77 |
| 4 | 1,794.76 | 4,834.78 |
| 8 | 2,750.64 | 5,534.75 |
| 12 | 2,848.89 | 5,524.11 |
