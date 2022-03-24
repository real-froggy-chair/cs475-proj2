/* resched.c - resched */

#include <xinu.h>

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

	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	// Old process returns here when resumed
	return;
}
