# cs475-proj2

Xinu Project 2
https://davidtchiu.github.io/teaching/cs475/proj2/

Decription of Aging System: 
Every process is assigned a priority value and a timesSched value.
The priority value works as expected and is set at process creation.
timesSched is initialized to zero, and, every time a process is rescheduled on the ready queue, it is increased by one. 
The system also tracks the least-scheduled process's PID in leastSched, which is updated each resched() call. Every resched() call, the priority of the least aged process (leastSched) increases by two, is dequeued, and then subsequently re-enqued, to update its position in the queue reflective of the new priority. 