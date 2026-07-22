Questions
1. First, open two separate terminal connections to the same machine,
so that you can easily run something in one window and the other.
Now, in one window, run vmstat 1, which shows statistics about
machine usage every second. Read the man page, the associated
README, and any other information you need so that you can understand its output. Leave this window running vmstat for the
rest of the exercises below.
Now, we will run the program mem.c but with very little memory
usage. This can be accomplished by typing ./mem 1 (which uses
only 1 MB of memory). How do the CPU usage statistics change
when running mem? Do the numbers in the user time column
make sense? How does this change when running more than one
instance of mem at once?

When running m:
More Context Switches because of the scheduling between the new procs. 


User Time Column:

Yes, some of the mem.c code is running and a few parts are user code, so the time is more. B

us: Time spent running non-kernel code.

When running multiple instances: 

Same, way more context switches, less free memory, bigger buffers and more caching. si/so is still 0. User times increase further, since more user code is running. More procs running. 




2. Let’s now start looking at some of the memory statistics while running mem. We’ll focus on two columns: swpd (the amount of virtual memory used) and free (the amount of idle memory). Run
./mem 1024 (which allocates 1024 MB) and watch how these values change. Then kill the running program (by typing control-c)
and watch again how the values change. What do you notice about
the values? In particular, how does the free column change when
the program exits? Does the amount of free memory increase by the
expected amount when mem exits?

VM:
Stays 0, since no swap memory is is needed. 

Free:
It goes from 30305000 (round about) back to around 31,350,000

So it loses the 1 GB of RAM, as expected.



3. We’ll next look at the swap columns (si and so), which indicate
how much swapping is taking place to and from the disk. Of course,
to activate these, you’ll need to run mem with large amounts of
memory. First, examine how much free memory is on your Linux
system (for example, by typing cat /proc/meminfo; type man
proc for details on the /proc file system and the types of information you can find there). One of the first entries in /proc/meminfo
is the total amount of memory in your system. Let’s assume it’s
something like 8 GB of memory; if so, start by running mem 4000
(about 4 GB) and watching the swap in/out columns. Do they ever
give non-zero values? Then, try with 5000, 6000, etc. What happens to these values as the program enters the second loop (and
beyond), as compared to the first loop? How much data (total)
are swapped in and out during the second, third, and subsequent
loops? (do the numbers make sense?)



4. Do the same experiments as above, but now watch the other statistics (such as CPU utilization, and block I/O statistics). How do they
change when mem is running?


5. Now let’s examine performance. Pick an input for mem that comfortably fits in memory (say 4000 if the amount of memory on the
system is 8 GB). How long does loop 0 take (and subsequent loops
1, 2, etc.)? Now pick a size comfortably beyond the size of memory
(say 12000 again assuming 8 GB of memory). How long do the
loops take here? How do the bandwidth numbers compare? How
different is performance when constantly swapping versus fitting
everything comfortably in memory? Can you make a graph, with
the size of memory used by mem on the x-axis, and the bandwidth of
accessing said memory on the y-axis? Finally, how does the performance of the first loop compare to that of subsequent loops, for both
the case where everything fits in memory and where it doesn’t?


6. Swap space isn’t infinite. You can use the tool swapon with the -s
flag to see how much swap space is available. What happens if you
try to run mem with increasingly large values, beyond what seems
to be available in swap? At what point does the memory allocation
fail?


7. Finally, if you’re advanced, you can configure your system to use
different swap devices using swapon and swapoff. Read the man
pages for details. If you have access to different hardware, see how
the performance of swapping changes when swapping to a classic
hard drive, a flash-based SSD, and even a RAID array. How much
can swapping performance be improved via newer devices? How
close can you get to in-memory performance?


Man Page:

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
