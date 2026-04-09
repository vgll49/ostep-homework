# OSTEP – Lottery Scheduling Homework

## 1. Random seeds (3 jobs)
**Question:**  
Compute the solutions for simulations with 3 jobs and random seeds of 1, 2, and 3.

### Seed 1
Command:
    ./lottery.py -j 3 -s 1 

Result: j2, j0, j1, j2, j2, j1 till finished

### Seed 2
Command:
    ./lottery.py -j 3 -s 2 

Result:
- j2 (7), j0, (8), j0(7),j2,j0,j1,j0,j2,j0,j0,j0,j1,j0,j0,j1,j2,j1,j1,j1,j2,j1,j1,j2

---

## 2. Ticket imbalance
**Question:**  
Two jobs: length 10, tickets 1 vs 100 (-l 10:1,10:100). What happens?

Command:
    ./lottery.py -l 10:1,10:100 -c

Explanation:
- j0 can run before j1 completes, but rarely
- lottery scheduling with imbalanced tickets is really unfair 

---

## 3. Fairness (equal tickets)
**Question:**  
Two jobs length 100, tickets 100 vs 100. How unfair is the scheduler?

Command:
    ./lottery.py -l 100:100,100:100 -s X -c

Runs:
- Seed0 j0:192,j1:200→ difference:
- Seed1 j0:200,j1:196 → difference:
- Seed2 j0:200j1:190 → difference:

Observation:
- Not really unfair, the more runs the fairer the scheduler will be with an balanced number of tickets

---

## 4. Effect of quantum (-q)
**Question:**  
How does fairness change with larger quantum?

Command:
    ./lottery.py -l 100:100,100:100 -q __ -c

Observation:
- the bigger the q the more weight has a single run, the more unfair it can be in general
- the fewer lottery decicions, the more "luck" is a factor

---

## 5. Graph / further exploration
**Question:**  
Recreate graph from chapter. What else to explore?

Answer:
- lottery will get fairer, the longer the jobs are
- stride scheduler would always be fair

---
