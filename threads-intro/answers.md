# Questions

## 1. `loop.s` — Single Thread

Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments:

`./x86.py -t 1 -p loop.s -i 100 -R dx`

This specifies a single thread, an interrupt every 100 instructions, and tracing of register `%dx`. What will `%dx` be during the run? Use the `-c` flag to check your answers; the answers, on the left, show the value of the register (or memory value) after the instruction on the right has run.

| dx | Thread 0 |
|---:|---|
| 0 | |
| -1 | `1000 sub $1,%dx` |
| -1 | `1001 test $0,%dx` |
| -1 | `1002 jgte .top` |
| -1 | `1003 halt` |

## 2. Same code, different flags

`./x86.py -p loop.s -t 2 -i 100 -a dx=3,dx=3 -R dx`

This specifies two threads, and initializes each `%dx` to 3. What values will `%dx` see? Run with `-c` to check. Does the presence of multiple threads affect your calculations? Is there a race in this code?

### Answer

The dx register will just go from 3,2,1,0,-1 in both threads. They share the virtual adress space, but not registers.

## 3. Random interrupt intervals

`./x86.py -p loop.s -t 2 -i 3 -r -R dx -a dx=3,dx=3`

This makes the interrupt interval small/random; use different seeds (`-s`) to see different interleavings. Does the interrupt frequency change anything?

### Answer

This will change the scheduling of each thread, but it won't change the value of the `%dx` register.

## 4. `looping-race-nolock.s` — Single Thread

`./x86.py -p looping-race-nolock.s -t 1 -M 2000`

What is value (i.e., at memory address 2000) throughout the run? Use `-c` to check.

### Answer

Throughout the run it will start with 0 and get incremented through 1 and then halt.

## 5. Multiple iterations/threads

`./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000`

Why does each thread loop three times? What is final value of value?

### Answer

Each thread loops three times because the `%bx` register is not shared, as the volatile var (adress 2000). The final value is 6.

## 6. Random interrupt intervals

`./x86.py -p looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0`

with different seeds (`-s 1`, `-s 2`, etc.).

Can you tell by looking at the thread interleaving what the final value of value will be? Does the timing of the interrupt matter? Where can it safely occur? Where not? In other words, where is the critical section exactly?

### Answer

The timer of the interrupt matters a lot. If it appears in the critical section, the value might get read at the same value from both threads, and only incremented once, thus missing an incrementation.

The critical section is the first three lines of the code:

    1000 mov 2000, %ax
    1001 add $1, %ax
    1002 mov %ax, 2000

The register has to savely get read, incremented and updated.

### Seed 0

Safely incremented, 2.

### Seed 1

Interrupt in the critical section, value will be 1.

### Seed 2

no critical section interrupt, 2.

## 7. Fixed interrupt intervals

`./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1`

What will the final value of the shared variable value be? What about when you change `-i 2`, `-i 3`, etc.? For which interrupt intervals does the program give the “correct” answer?

### Answer

The correct answer will be 3, because like this the critical section is always atomic because the critical section includes 3 instructions. 1 and 2 to the critical sections are interrupted.

## 8. More loops

Run the same for more loops (e.g., set `-a bx=100`). What interrupt intervals (`-i`) lead to a correct outcome? Which intervals are surprising?

### Answer

3 and 6 will work. Basically of multiples of 3 will work thus it does not interrupt the critical section.

## 9. `wait-for-me.s`

`./x86.py -p wait-for-me.s -a ax=1,ax=0 -R ax -M 2000`

This sets the `%ax` register to 1 for thread 0, and 0 for thread 1, and watches `%ax` and memory location 2000. How should the code behave? How is the value at location 2000 being used by the threads? What will its final value be?

### Answer

Final value will be 1. In t0 it meets the condition and thus je is not done, the instruction does set 1 into adress 2000. t1 then does not meet the test  and jumps to .singnaller and reads the value from the adresse. Thread0 is the signaller and thread1 the waiter.

## 10. Switch the inputs

`./x86.py -p wait-for-me.s -a ax=0,ax=1 -R ax -M 2000`

How do the threads behave? What is thread 0 doing? How would changing the interrupt interval (e.g., -i 1000, or perhaps to use random intervals) change the trace outcome? Is the program efficiently using the CPU?

### Answer
Wwithout changing the interrupt t0 runs till the first interrupt occurs because it waits for the second thread to set 1 into the the adress. It will always wait till the signaller could do mov $1, 2000 (set 1 in 2000.) The programm is not at all using the CPU in an efficient manner because it does busy waiting.