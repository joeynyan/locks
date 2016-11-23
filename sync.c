/*
 *
 *
 *
 *
 */

#define _REENTRANT
#define MAXBACKOFF 1000000

#include "sync.h"


/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	lock->status = 0;
	lock->timesLocked = 0;
	lock->threadID = -5;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
	return 0;
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		lock->timesLocked--;
		if (lock->timesLocked == 0) {
			lock->threadID = -5;
			lock->status = 0;
		}
	}
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	//keep trying til you get the lock
	if(my_spinlock_trylock(lock) == 1) {
		lock->timesLocked++;
	} else {
		while(tas(&lock->status)!=0){}
		lock->threadID = syscall(SYS_gettid);
		lock->timesLocked++;
	}
	return 0;
}


int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	while(1) {
		if(my_spinlock_trylock(lock) == 1) {
			lock->timesLocked++;
			break;
		} else
		if(lock->status == 0) {
			if(tas(&lock->status) == 0) {
				lock->threadID = syscall(SYS_gettid);
				lock->timesLocked++;
				break;
			}
		}
	}
	return 0;
}

int my_spinlock_trylock(my_spinlock_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		return 1;
	} else {
		return 0;
	}
}


/*
 * Mutex Routines
 */

int my_mutex_init(my_mutex_t *lock)
{
	lock->status = 0;
	lock->backoff = rand()%10;
	lock->timesLocked = 0;
	lock->threadID = -5;
}

int my_mutex_destroy(my_mutex_t *lock)
{
	return 0;
}

int my_mutex_unlock(my_mutex_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		lock->timesLocked--;
		if (lock->timesLocked == 0) {
			lock->threadID = -5;
			lock->status = 0;
		}
	}
}

int my_mutex_lock(my_mutex_t *lock)
{
	if (my_mutex_trylock(lock) == 1) {
		lock->timesLocked++;
	} else {
		while(tas(&lock->status) != 0) {
			//nanosleep usleep sleep
			lock->wait.tv_nsec = lock->backoff;
			nanosleep(&lock->wait,NULL);
			if (lock->backoff < MAXBACKOFF)
			{
				lock->backoff = lock->backoff*lock->backoff;
			} else
			{
				lock->backoff = rand()%10;
			}
		}
		lock->threadID = syscall(SYS_gettid);
		lock->timesLocked++;
		lock->backoff = rand()%10;
	}
	return 0;
}

int my_mutex_trylock(my_mutex_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * Queue lock
 */
// volatile long unsigned int ticket = 0;
// volatile long unsigned int nowServing = 0;
int my_queuelock_init(my_queuelock_t *lock)
{
	lock->ticket = 0;
	lock->nowServing = 0;
	lock->timesLocked = 0;
	lock->threadID = -5;
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
	return 0;
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		lock->timesLocked--;
		if (lock->timesLocked == 0) {
			lock->threadID = -5;
			lock->nowServing = lock->nowServing+1;
		}
	}
}

int my_queuelock_lock(my_queuelock_t *lock)
{
	if(my_queuelock_trylock(lock) == 1) {
		lock->timesLocked++;
	} else {
		int myTicket = lock->ticket;
		while(cas(&lock->ticket, myTicket, (lock->ticket+1)) != myTicket){
			myTicket = lock->ticket;
		}
		while(lock->nowServing != myTicket){}
		lock->threadID = syscall(SYS_gettid);
		lock->timesLocked++;
	}
	return 0;
}

int my_queuelock_trylock(my_queuelock_t *lock)
{
	if (lock->threadID == syscall(SYS_gettid)) {
		return 1;
	} else {
		return 0;
	}
}
