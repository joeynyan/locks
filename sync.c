/*
 *
 *
 *
 *
 */

#define _REENTRANT

#include "sync.h"


/*
 * Spinlock routines
 */

int my_spinlock_init(my_spinlock_t *lock)
{
	lock->status = 0;
}

int my_spinlock_destroy(my_spinlock_t *lock)
{
	return 0;
}

int my_spinlock_unlock(my_spinlock_t *lock)
{
	lock->status = 0;
}

int my_spinlock_lockTAS(my_spinlock_t *lock)
{
	//keep trying til you get the lock
	while(tas(&lock->status!=0){}
	return 0;
}


int my_spinlock_lockTTAS(my_spinlock_t *lock)
{
	while(1) {
		if(lock->status == 0) {
			if(tas(&lock->status) == 0) {
				break;
			}
		}
	}
	return 0;
}

int my_spinlock_trylock(my_spinlock_t *lock)
{
}


/*
 * Mutex Routines
 */

int my_mutex_init(my_mutex_t *lock)
{
}

int my_mutex_destroy(my_mutex_t *lock)
{
}

int my_mutex_unlock(my_mutex_t *lock)
{
}

int my_mutex_lock(my_mutex_t *lock)
{
}

int my_mutex_trylock(my_mutex_t *lock)
{
}

/*
 * Queue lock
 */

int my_queuelock_init(my_queuelock_t *lock)
{
}

int my_queuelock_destroy(my_queuelock_t *lock)
{
}

int my_queuelock_unlock(my_queuelock_t *lock)
{
}

int my_queuelock_lock(my_queuelock_t *lock)
{
}

int my_queuelock_trylock(my_queuelock_t *lock)
{
}


