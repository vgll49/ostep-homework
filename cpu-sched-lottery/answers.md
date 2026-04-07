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

Result:
- Job 0 runs before Job 1 finishes?:
- How often:
- Who finishes first:

Explanation:
- 

---

## 3. Fairness (equal tickets)
**Question:**  
Two jobs length 100, tickets 100 vs 100. How unfair is the scheduler?

Command:
    ./lottery.py -l 100:100,100:100 -s X -c

Runs:
- Seed __ → difference:
- Seed __ → difference:
- Seed __ → difference:

Observation:
- 

---

## 4. Effect of quantum (-q)
**Question:**  
How does fairness change with larger quantum?

Command:
    ./lottery.py -l 100:100,100:100 -q __ -c

Result:
- 

Observation:
- 

---

## 5. Graph / further exploration
**Question:**  
Recreate graph from chapter. What else to explore?

Graph idea:
- X-axis:
- Y-axis:

Experiments:
- 

Stride scheduler (comparison):
- Expected behavior:
- 

---

## Notes
- 