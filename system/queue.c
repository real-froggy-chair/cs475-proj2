/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	//TODO - print all contents from head to tail
	kprintf("[");
	if (nonempty(q))
	{
		struct qentry *curr = q->head;
		while (curr != NULL)
		{
			kprintf("(pid=%u)", curr->pid);
			curr = curr->next;
		}
	}
	kprintf("]\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	isempty(struct queue *q)
{
	//TODO - trivial
	return (q->size == 0);
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	nonempty(struct queue *q)
{
	//TODO - trivial (but don't just check q's size because it could be NULL)
	return (q == NULL || q->size > 0);
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return 1 if true, 0 otherwise
 */
bool8	isfull(struct queue *q)
{
	//TODO - check if there are at least NPROC processes in the queue
	return (q->size >= NPROC);
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q, int32 key)
{
	if (isfull(q) || isbadpid(pid)) {
		kprintf("ERROR!\n");
		return SYSERR;
	}

	//TODO - allocate space on heap for a new qentry
	struct qentry *newEntry = (struct qentry*) malloc(sizeof(struct qentry));

	//TODO - initialize the new QEntry
	newEntry->pid  = pid;
	newEntry->prev = NULL;
	newEntry->next = NULL;
	newEntry->key  = key;

	// insert into queue

	bool inserted = false;
	if (isempty(q)){
		q->head = newEntry;
		q->tail = newEntry;
		inserted = true;
	}
	struct qentry *curr = q->head;
	struct qentry *temp;
	while (inserted == false && curr != NULL){
		int32 currKey = curr->key;
		if (key > currKey){
			if (curr != q->head){
				curr->prev->next = newEntry;
			}
			newEntry->prev = curr->prev;
			newEntry->next = curr;
			curr->prev = newEntry;
			if (curr == q->head){
				// update Queue head to point to new entry if needed
				q->head = newEntry;
			}
			inserted = true;
		}
		else if (key == currKey && curr != q->tail){
			// maintain FIFO: check if next entry in queue has same priority
			temp = curr->next;
			if (temp->key != currKey){
				temp->prev = newEntry;
				newEntry->next = temp;
				newEntry->prev = curr;
				curr->next = newEntry;
				inserted = true;
			}
		}
		else if (key <= currKey && curr == q->tail){
			newEntry->prev = q->tail;
			newEntry->next = NULL;
			q->tail->next = newEntry;
			q->tail = newEntry;
			inserted = true;
		}
		curr = curr->next;
	}

	//update Queue head if needed
	if (q->head == NULL){
		q->head = newEntry;
	}

	//update queue size
	q->size++;

	return pid;
}

/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
	pid32	pid;	//ID of process removed

	//TODO - return EMPTY if queue is empty
	if (isempty(q)) {
		return EMPTY;
	}

	//TODO - get the head entry of the queue

	//TODO - unlink the head entry from the rest

	//TODO - free up the space on the heap

	//save pointer to head entry
	struct qentry *head = q->head;
	struct qentry *newHead = head->next;

	//save pid of head entry
	pid = head->pid;

	//unlink head from queue
	if (newHead != NULL)
		newHead->prev = NULL;
	else
		q->tail = NULL;

	//update queue to point head pointer at newhead
	q->head = newHead;

	//decrement size of queue
	q->size--;

	//deallocate head entry
	free(head, sizeof(struct qentry));

	return pid;
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if (isempty(q))
		return NULL;

	//TODO - find the qentry with the given pid
	struct qentry *currEntry = q->head;
	while(currEntry != NULL && currEntry->pid != pid)
		currEntry = currEntry->next;

	//TODO - return a pointer to it
	return currEntry;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//TODO - return EMPTY if queue is empty

	//TODO - remove process from head of queue and return its pid

	if (isempty(q)) {
		return EMPTY;
	}

	return dequeue(q);
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if (isempty(q)) {
		return EMPTY;
	}

	//TODO - remove process from tail of queue and return its pid
	return remove(q->tail->pid, q);
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32 remove(pid32 pid, struct queue *q)
{
  // TODO - return EMPTY if queue is empty
  if (isempty(q))
  {
    return EMPTY;
  }

  // TODO - remove process identified by pid parameter from the queue and return its pid

  // find the entry with pid
  struct qentry *currEntry = q->head;
  while (currEntry != NULL)
  {
    // found it!
    if (currEntry->pid == pid)
    {
      // unlink: find next and prev entries
      struct qentry *next = currEntry->next;
      struct qentry *prev = currEntry->prev;
      if (next != NULL)
        next->prev = prev;
      if (prev != NULL)
        prev->next = next;

      // update queue structure
      if (currEntry == q->head)
        q->head = next;
      if (currEntry == q->tail)
        q->tail = prev;
      q->size--;

      // deallocate current entry
      free(currEntry, sizeof(struct qentry));
      return pid;
    }
    currEntry = currEntry->next;
  }

  // TODO - if pid does not exist in the queue, return SYSERR
  return SYSERR;
}
