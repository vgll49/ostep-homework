# Questions

### 1. First build `main-race.c`. Examine the code so you can see the (hopefully obvious) data race in the code. Now run Helgrind (by typing `valgrind --tool=helgrind ./main-race`) to see how it reports the race. Does it point to the right lines of code? What other information does it give to you?

It points to a possible data race in line 15, which is correct. It also gives information about the creation of the threads and the lines where they were created.

    ==5236== Possible data race during write of size 4 at 0x10C014 by thread #1
    ==5236== Locks held: none
    ==5236==    at 0x10923F: main (main-race.c:15)
    ==5236==
    ==5236== This conflicts with a previous write of size 4 by thread #2
    ==5236== Locks held: none
    ==5236==    at 0x1091BE: worker (main-race.c:8)
    ==5236==    by 0x4854B7A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
    ==5236==    by 0x4905B83: start_thread (pthread_create.c:447)
    ==5236==    by 0x4992B63: clone (clone.S:100)
    ==5236==  Address 0x10c014 is 0 bytes inside data symbol "balance"

### 2. What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does Helgrind report in each of these cases?

**Case 1: Removing `balance++` from the main thread**

Nothing is detected:

    ==6205== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

**Case 2: Adding a lock around one update**

The race still exists.

**Case 3: Adding a lock around both updates**

The race is fixed because both critical sections are protected from race conditions.

### 3. Now let’s look at `main-deadlock.c`. Examine the code. This code has a problem known as deadlock (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?

    p1: lock m1        → m1 belongs to p1
    p2: lock m2        → m2 belongs to p2

    p1: lock m2        → waits because p2 owns m2
    p2: lock m1        → waits because p1 owns m1

### 4. Now run Helgrind on this code. What does Helgrind report?

A small part of the relevant output:

    ==22572== Thread #3: lock order "0x10C040 before 0x10C080" violated

It shows the exact memory addresses of the locks and the lock acquisition orders that are in conflict with each other.

### 5. Now run Helgrind on `main-deadlock-global.c`. Examine the code; does it have the same problem that `main-deadlock.c` has? Should Helgrind be reporting the same error? What does this tell you about tools like Helgrind?

It first locks the acquisition of the two other locks and thus basically creates a critical section for the procedure.

Helgrind still reports an error because it does not see the specific execution and therefore still reports it because it sees a lock acquisition order that is different.

### 6. Let’s next look at `main-signal.c`. This code uses a variable (`done`) to signal that the child is done and that the parent can now continue. Why is this code inefficient? (What does the parent end up spending its time doing, particularly if the child thread takes a long time to complete?)

Because the parent spins in the `while` loop the whole time. It repeatedly checks `done` and wastes CPU cycles while the child thread is still running.

### 7. Now run Helgrind on this program. What does it report? Is the code correct?

    ==34603== Possible data race during read of size 4 at 0x10C014 by thread #1

The code is not thread-safe. The main thread can read `done` while the worker thread is changing it, thus causing the described data race.

### 8. Now look at a slightly modified version of the code, which is found in `main-signal-cv.c`. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?

Now it uses a condition variable instead of spinning. The thread basically just "sleeps" until it is woken by the signal and then checks the condition again. Thus, it is both safer and more efficient because no CPU cycles are wasted.

### 9. Once again run Helgrind on `main-signal-cv`. Does it report any errors?

No, it works as expected.

    ==35737== Command: ./main-signal-cv
    ==
    this should print first
    this should print last
    ==
    ==35737== Use --his