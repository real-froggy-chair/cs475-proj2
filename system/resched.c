/* resched.c - resched */

#include <xinu.h>
#include <stdbool.h>

/**
 * Reschedule processor to next ready process
 *
 */
void	resched(void)		// assumes interrupts are disabled
{
	struct procent *ptold;	// ptr to table entry for old process
	struct procent *ptnew;	// ptr to table entry for new process

	// If rescheduling is deferred, record attempt and return
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	// Point to process table entry for the current (old) process
	// DC REMOVE

	ptold = &proctab[currpid];
	pid32 ptoldpid = currpid; // save pid of ptold

		// TODO - check ptold's state. If it's running, put it on the ready queue and change state to ready
        if (ptold->prstate == PR_CURR) {
                // Old process got preempted; place back on ready queue
                ptold->prstate = PR_READY;
				pri16 key = ptold->prprio;
                enqueue(currpid, readyqueue, key);
        }
		
		// TODO - set currpid to reflect new running process' PID
        currpid = dequeue(readyqueue);
		// TODO - dequeue next process off the ready queue and point ptnew to it
        ptnew = &proctab[currpid];
		// TODO - change its state to "current" (i.e., running)
        ptnew->prstate = PR_CURR;
		if (AGING == TRUE && prcount >= 2 && readyqueue->size > 1){
			// increase variable tracking number of times ptnew has been scheduled
			ptnew->timessched++;
			// increase priority of least scheduled process
			struct procent *ptleast; // ptr to table entry for least scheduled process
			bool b = false; // tracks whether first process to investigate has been found 
			pid32 leastSched = NULL;
			struct qentry *curr = readyqueue->head->next;
			while (b == false && curr != NULL){ // find process in queue that is not ptold
				if (curr->pid != ptoldpid){
					leastSched = curr->pid;
					b = true;
				}
				curr = curr->next;
			}
			curr = readyqueue->head->next;
			while (curr != NULL){ // find process that has been scheduled the least
				if (curr->pid != ptoldpid){
					if (proctab[curr->pid].timessched < proctab[leastSched].timessched){
						leastSched = curr->pid;
					}
				}
				curr = curr->next;
			}
			if (leastSched != ptoldpid && leastSched != NULL && curr->pid > 1){
				ptleast = &proctab[leastSched];
				ptleast->prprio = ptleast->prprio + 5;
				remove(leastSched, readyqueue);
				pri16 agekey = ptleast->prprio;
				enqueue(leastSched, readyqueue, agekey);
			}
		}

	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	// Old process returns here when resumed
	return;
}
