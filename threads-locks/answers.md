# OSTEP Threads & Locks

## 1. Examine `flag.s`. This code “implements” locking with a single memory flag. Can you understand the assembly?

**A:** Shared flag variable that gets locked/unlocked around the critical section.

---

## 2. When you run with the defaults, does `flag.s` work? Use the `-M` and `-R` flags to trace variables and registers (and turn on `-c` to see their values). Can you predict what value will end up in `flag`?

**A:** Command is:

`python3 x86.py -p flag.s -t 2 -a bx=1 -M flag,count -R ax,bx -c`

The flag will be zero because the lock is released.

---

## 3. Change the value of the register `%bx` with the `-a` flag (e.g., `-a bx=2,bx=2` if you are running just two threads). What does the code do? How does it change your answer for the question above?

**A:** The flag will still be `0` in the end. The loop is just increased, so it loops two times for each thread.

---

## 4. Set `bx` to a high value for each thread, and then use the `-i` flag to generate different interrupt frequencies; what values lead to bad outcomes? Which lead to good outcomes?

**Command:**

`python3 x86.py -p flag.s -a bx=10,bx=10 -i 9 -t 2 -M flag,count -R ax,bx -c`

**A:** Values that interrupt the flag acquisition lead to bad behavior because this is not an atomic operation. If the interrupt frequency interrupts the lock acquisition, it can happen that both threads think the flag is `0` and can therefore acquire the lock.

See:

Both threads acquire the lock, then an interrupt happens, and both have the value `0`. Thus, inconsistencies arise.

    0     9       9     5                            1008 sub  $1, %bx
    0     9       1     5   ------ Interrupt ------  ------ Interrupt ------
    0     9       1     5   1001 test $0, %ax
    0     9       1     5   1002 jne  .acquire
    0     9       0     5   1000 mov  flag, %ax
    0     9       0     5   1001 test $0, %ax
    0     9       0     5   1002 jne  .acquire
    0     9       9     5   ------ Interrupt ------  ------ Interrupt ------
    0     9       9     5                            1009 test $0, %bx
    0     9       9     5                            1010 jgt .top
    0     9       0     5                            1000 mov  flag, %ax
    0     9       0     5                            1001 test $0, %ax
    0     9       0     5                            1002 jne  .acquire
    0     9       0     5   ------ Interrupt ------  ------ Interrupt ------
    1     9       0     5   1003 mov  $1, flag
    1     9       9     5   1004 mov  count, %ax
    1     9      10     5   1005 add  $1, %ax
    1    10      10     5   1006 mov  %ax, count
    0    10      10     5   1007 mov  $0, flag
    0    10       0     5   ------ Interrupt ------  ------ Interrupt ------
    1    10       0     5                            1003 mov  $1, flag

---

## 5. Now let’s look at the program `test-and-set.s`. First, try to understand the code, which uses the `xchg` instruction to build a simple locking primitive. How is the lock acquire written? How about lock release?

**A:** Inside the acquisition, the thread puts `1` in the `%ax` register and then uses the atomic `xchg` to put the value into the mutex and gets what was inside the mutex before the swap. If the value is not `0`, then the lock was locked and it tries again.

For releasing, it just puts `0` inside the mutex.

---

## 6. Now run the code, changing the value of the interrupt interval (`-i`) again, and making sure to loop for a number of times. Does the code always work as expected? Does it sometimes lead to an inefficient use of the CPU? How could you quantify that?

**A:** It always works properly, even with large iterations and different interrupt timers. It is actually “spinning” around the mutex because it always tries again, even if the lock is held by another thread, thus wasting CPU cycles as long as no interrupt occurs.

I could use the `.acquire` label and count how often an acquisition was successful.

---

## 7. Use the `-P` flag to generate specific tests of the locking code. For example, run a schedule that grabs the lock in the first thread, but then tries to acquire it in the second. Does the right thing happen? What else should you test?

### Test 1

`python3 x86.py -p test-and-set.s -a bx=5,bx=5 -t 2 -M mutex,count -R ax,bx -P 000011111 -c`

Here, T1 does not enter the critical section.

T1 gets the lock first, T0 tries: basically the same as T1.

Both threads try to acquire the lock:
The one that gets `xchg` first gets the lock.

One releases while the other is waiting: this also works with:

`python3 x86.py -p test-and-set.s -a bx=5,bx=5 -t 2 -M mutex,count -R ax,bx -P 000011111 -c`

    0     0       0     1
    0     0       1     1   1000 mov  $1, %ax
    1     0       0     1   1001 xchg %ax, mutex
    1     0       0     1   1002 test $0, %ax
    1     0       0     1   1003 jne  .acquire
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       1     1                            1000 mov  $1, %ax
    1     0       1     1                            1001 xchg %ax, mutex
    1     0       1     1                            1002 test $0, %ax
    1     0       1     1                            1003 jne  .acquire
    1     0       1     1                            1000 mov  $1, %ax
    1     0       0     1   ------ Interrupt ------  ------ Interrupt ------
    1     0       0     1   1004 mov  count, %ax
    1     0       1     1   1005 add  $1, %ax
    1     1       1     1   1006 mov  %ax, count
    0     1       1     1   1007 mov  $0, mutex
    0     1       1     1   ------ Interrupt ------  ------ Interrupt ------
    1     1       0     1                            1001 xchg %ax, mutex
    1     1       0     1                            1002 test $0, %ax
    1     1       0     1                            1003 jne  .acquire
    1     1       1     1                            1004 mov  count, %ax
    1     1       2     1                            1005 add  $1, %ax

---

## 8. Now let’s look at the code in `peterson.s`, which implements Peterson’s algorithm (mentioned in a sidebar in the text). Study the code and see if you can make sense of it.

**Turn:** Who has priority?

`turn = 1 → T1 has priority`  
`turn = 0 → T0 has priority`

---

## 9. Now run the code with different values of `-i`. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using `-a bx=0,bx=1` for example) as the code assumes it.

**A:** Depending on the `-i` value, you can see different execution paths. For example, a thread may enter the critical section directly after `.spin1`, or it may have to continue to `.spin2` if the other thread has declared its intent to enter. The interrupt interval also affects how long the waiting thread spends spinning before it can enter the critical section.

---

## 10. Can you control the scheduling (with the `-P` flag) to “prove” that the code works? What are the different cases you should show hold? Think about mutual exclusion and deadlock avoidance.

**A:**

- T0 wants to enter alone.
- T1 wants to enter alone.
- Both try to acquire the lock at the same time.
- The cases where the shared variables are accessed are the ones that have to be looked at.

For mutual exclusion, when both threads want to enter at the same time, only one should enter the critical section while the other waits.

For deadlock avoidance, the waiting thread should eventually be able to enter after the first thread leaves the critical section.

---

## 11. Now study the code for the ticket lock in `ticket.s`. Does it match the code in the chapter? Then run with the following flags: `-a bx=1000,bx=1000` (causing each thread to loop through the critical section 1000 times). Watch what happens; do the threads spend much time spin-waiting for the lock?

**A:** In the code in the chapter, a thread in the lock acquisition basically just generates a ticket via `FetchAndAdd` and then spins until its ticket is “chosen” (its turn matches the ticket number) by the unlock incrementing `turn`. The implementation in here basically does the same. The lock itself works, but it actually does spend a lot of time spin-locking.

---

## 12. How does the code behave as you add more threads?

**A:** The time the threads spend spinning increases even more. It can happen that they spend their whole time slice spinning.

---

## 13. Now examine `yield.s`, in which a `yield` instruction enables one thread to yield control of the CPU (realistically, this would be an OS primitive, but for simplicity, we assume an instruction does the task). Find a scenario where `test-and-set.s` wastes cycles spinning, but `yield.s` does not. How many instructions are saved? In what scenarios do these savings arise?

**A:** The savings depend on the interrupt frequency and the scheduling. In `test-and-set.s`, a thread that fails to acquire the lock keeps spinning and executes more instructions until it is interrupted. In `yield.s`, the thread immediately yields the CPU after a failed attempt, allowing the other thread to run.

The savings arise when a thread fails to acquire the lock and the lock holder is able to run and release it. In these cases, `yield.s` avoids the unnecessary spinning instructions that `test-and-set.s` would execute. The larger the possible time slice, the more instructions can potentially be saved.

---

## 14. Finally, examine `test-and-test-and-set.s`. What does this lock do? What kind of savings does it introduce as compared to `test-and-set.s`?

**A:** The test-and-test-and-set lock first checks the mutex with a normal load. If the lock is already held, the thread keeps spinning without executing the more expensive atomic `xchg` instruction. Only when the mutex appears to be free does it perform the atomic exchange to actually acquire the lock.

Compared to `test-and-set.s`, this reduces the number of expensive atomic operations performed while the lock is held, especially when many threads are spinning on the same lock.