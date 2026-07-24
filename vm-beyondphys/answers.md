# Questions

## 1. vmstat and CPU usage

First, open two separate terminal connections to the same machine, run `vmstat 1`, and observe the CPU usage statistics while running `mem`.

### When running mem

- More context switches because of the scheduling between the new processes.

### User Time Column

Yes, some of the `mem.c` code is running and a few parts are user code, so the time is more.

`us`: Time spent running non-kernel code.

### When running multiple instances

- Same, way more context switches.
- Less free memory.
- Bigger buffers and more caching.
- `si/so` is still 0.
- User times increase further, since more user code is running.
- More processes running.

---

## 2. Memory statistics while running mem

Observe `swpd` and `free` while running `./mem 1024`.

### VM

- Stays 0, since no swap memory is needed.

### Free

- It goes from around `30305000` back to around `31350000`.

So it loses the 1 GB of RAM, as expected.

---

## 3. Swap activity

Observe `si` and `so` while running `mem` with larger memory values.

### System Memory

```
MemTotal: around 32GB

SwapTotal: around 8 GB
```

### When using 16 GB

```
SI: 0
SO: 0
```

Still nothing gets swapped in and out.

### When using 24 GB

```
SI: 0
SO: 0
```

### When using 30 GB

- It starts swapping in and out sometimes.
- Values range from a few KB to 50,000 KB.

### When using 32 GB

- A lot gets swapped.
- `si/so` gets very high after each loop for a second.
- This may be because `mem` has to access pages that have been swapped out in previous iterations.

---

## 4. CPU utilization and block I/O

Observe CPU and block I/O statistics while running `mem`.

### 16 GB

**CPU:**

- More time of user code, less.

**Block I/O:**

- Some I/O operation blocks, but also on 0 a lot.

### 32 GB

**CPU:**

- CPU idle time is around the same.
- CPU `wa` time starts raising to single digit value because of the heavy I/O loads.

**Block I/O:**

- Bigger I/O block timers.
- They increase with swaps and are pretty similar.
- `SI` leads to `BI` timer.
- `SO` leads to `BO` timer.

---

## 5. Performance

Compare performance when memory fits vs. when swapping happens.

### 16 GB

**Time:**

- The first loop takes 12000 ms.
- The others roughly 2000 ms.

**Bandwidth:**

- Averages around 8000 MB/s.

### 30 GB

(Starts crashing on multiple loops if I use more.)

**Time:**

- First loop: 26000 ms.
- Subsequent loops average around 5000 ms.

**Bandwidth:**

- Between 5000 and 7000 MB/s.

If I would use more memory this trend would keep on.

Increasing memory pressure causes more swapping, which reduces effective bandwidth and increases loop execution time.

Under heavy enough memory pressure, performance can degrade dramatically due to thrashing.

---

## 6. Swap space limitations

Observe what happens when running `mem` beyond available swap space.

- 8 GB of swap memory.
- I can't correctly test it, because WSL will crash.
- I guess it will fail around 38 GB because of other things in memory, like:
  - kernel memory
  - page tables
  - WSL overhead
  - etc.

---

## 7. Different swap devices

Compare swapping performance between HDD, SSD, and RAID.

Swap speed will be much faster when switching from a hard drive to a flash-based SSD.

Improvement depends on the read/write speed of the device.




# Man Page (vsmtat):

   Procs
       r: The number of runnable processes (running or waiting for run time).
       b: The number of processes blocked waiting for I/O to complete.

   Memory
       These are affected by the --unit option.
       swpd: the amount of swap memory used.
       free: the amount of idle memory.
       buff: the amount of memory used as buffers.
       cache: the amount of memory used as cache.
       inact: the amount of inactive memory.  (-a option)
       active: the amount of active memory.  (-a option)

   Swap
       These are affected by the --unit option.
       si: Amount of memory swapped in from disk (/s).
       so: Amount of memory swapped to disk (/s).

   IO
       bi: Kibibyte received from a block device (KiB/s).
       bo: Kibibyte sent to a block device (KiB/s).

   System
       in: The number of interrupts per second, including the clock.
       cs: The number of context switches per second.

   CPU
       These are percentages of total CPU time.
       us: Time spent running non-kernel code.  (user time, including nice time)
       sy: Time spent running kernel code.  (system time)
       id: Time spent idle.  Prior to Linux 2.5.41, this includes IO-wait time.
       wa: Time spent waiting for IO.  Prior to Linux 2.5.41, included in idle.
       st: Time stolen from a virtual machine.  Prior to Linux 2.6.11, unknown.
       gu: Time spent running KVM guest code (guest time, including guest nice).
