# OSTEP – MLFQ Exercises

## 1. Run random problems (2 jobs, 2 queues)
**Question:**  
Run a few randomly-generated problems with just two jobs and two queues; compute the MLFQ execution trace for each. Make your life easier by limiting the length of each job and turning off I/Os.

---

## 2. Reproduce examples from the chapter
**Question:**  
How would you run the scheduler to reproduce each of the examples in the chapter?


**Command:**
8.2    ./mlfq -j1 -n3 -m200 -Q10,10,10 -A1,1,1 -M0  -c

8.3 left: ./mlfq -l 0,180,0:100,20,20 -n3 -m200 -Q10,10,10 -A1,1,1 -M0  -c

**Important parameters:**
- 

**Explanation:**
- 

---

## 3. Configure MLFQ as Round Robin
**Question:**  
How would you configure the scheduler parameters to behave just like a round-robin scheduler?

**Idea:**
- 

**Configuration:**
- Queues:
- Quantum:
- Boost:
- Other:

**Result:**
- 

---

## 4. Game the scheduler (-S)
**Question:**  
Craft a workload with two jobs and scheduler parameters so that one job takes advantage of the older Rules 4a and 4b (turned on with the -S flag) to game the scheduler and obtain 99% of the CPU over a particular time interval.

**Goal:**
- One job gets ~99% CPU

**Setup:**
- Job A:
- Job B:
- Parameters:

**What happens:**
- 

**Why it works:**
- 

---

## 5. Boost frequency (-B)
**Question:**  
Given a system with a quantum length of 10 ms in its highest queue, how often would you have to boost jobs back to the highest priority level (with the -B flag) in order to guarantee that a single long-running (and potentially-starving) job gets at least 5% of the CPU?

**Given:**
- Quantum = 10 ms
- Target = 5%

**Idea:**
- 

**Calculation:**
- 

**Result:**
- Boost every: ___ ms

---

## 6. Effect of -I (I/O behavior)
**Question:**  
One question that arises in scheduling is which end of a queue to add a job that just finished I/O; the -I flag changes this behavior for this scheduling simulator. Play around with some workloads and see if you can see the effect of this flag.

**Setup:**
- 

**Without -I:**
- 

**With -I:**
- 

**Difference:**
- 

---

## Final Notes
- 