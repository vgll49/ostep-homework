# CPU Scheduling Homework

## Definitions

- Response Time = first run - arrival time  
- Turnaround Time = completion time - arrival time  
- Waiting Time = turnaround time - job length  

Assumption: All jobs arrive at time 0.

---

## Question 1

Jobs: 200, 200, 200  
Schedulers: FIFO, SJF  

### FIFO

Order:


| Job | Response | Turnaround | Waiting |
|-----|----------|------------|---------|
| J1  |   0      |    200        |    0     |
| J2  |    200     |    400        |     200    |
| J3  |     400    |      600      |     400    |

### SJF

Order:


| Job | Response | Turnaround | Waiting |
|-----|----------|------------|---------|
| J1  |    0      | 200           | 0         |
| J2  |     200     |  400          | 200        |
| J3  |      400    |   600         | 400         |

---

## Question 2

Jobs: 100, 200, 300  

### FIFO

Order: 0,1,2



| Job | Response | Turnaround | Waiting |
|-----|----------|------------|---------|
| J1  |    0      |       100     |     0    |
| J2  |    100      |     300       |    100   |
| J3  |    300      |     600       |    300     |

### SJF

Order: 0, 1, 2


| Job | Response | Turnaround | Waiting |
|-----|----------|------------|---------|
| J1  |    0      |    100    | 0        |
| J2  |     100     |  300          |    100     |
| J3  |     300     |   600         |   300     |

---

## Question 3

Scheduler: RR (q = 1)

Timeline: 


| Job | Response | Turnaround | Waiting |
|-----|----------|------------|---------|
| J1  |    0      |     298       |   198      |
| J2  |    1      |     499       |    299    |
| J3  |    2     |      600      |    300    |

---

## Question 4

Answer: For worklods that appear ordered by runtime and thus dont require rearrangement by SJF.


---

## Question 5

Answer: If the Jobs arrive in ascending order of lenghts and the quantum is large enough, that no job is preempted, i.e. the quantum has to be at least as long as the second longest job.

---

## Question 6

Observation: The response time for longer jobs grows a lot, because each job must wait for all shorter jobs to complete. As the job lengths increase, the response time grows as the sumof the execution time of all preceding shorter jobs.



---

## Question 7

Observation: Response time grows linearly with Qantum (but only till quantum is so big it solves every job except longest in 1 q)


Worst-case formula: (n-1) * q