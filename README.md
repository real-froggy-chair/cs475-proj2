# cs475-proj2

Xinu Project 2
https://davidtchiu.github.io/teaching/cs475/proj2/

Decription of Aging Policy: Each process has a timesSched variable, which is initialized to zero. This variable tracks how many times a process has been scheduled, meaning it increments by one each time the process is dequeued in resched(). Each time resched() is called, the priority of the process that has the lowest timesSched value (meaning, the process that has been scheduled the least) is increased by five. This process is then removed from the queue and subsequently re-enqueued to update its position in the queue reflective of its new, higher priority. The nullproc process, the process that was just preempted, and the process that was just selected for scheduling are not incremented.
