/*
 * Joanna Che 301229906 jca241
 *
 * sync.h
 */

#ifndef _m_SYNC_H_
#define _m_SYNC_H_

#include "atomic_ops.h"

struct my_mutex_struct {
	volatile long unsigned int status;
	int value;
};

typedef struct my_mutex_struct my_mutex_t;


int my_mutex_init(my_mutex_t *lock);
int my_mutex_unlock(my_mutex_t *lock);
int my_mutex_destroy(my_mutex_t *lock);

int my_mutex_lock(my_mutex_t *lock);
int my_mutex_trylock(my_mutex_t *lock);

/* Spinlock starts here */

struct my_spinlock_struct {
	volatile long unsigned int status;
	int value;
	// fill me in
};

typedef struct my_spinlock_struct my_spinlock_t;

int my_spinlock_init(my_spinlock_t *lock);
int my_spinlock_destroy(my_spinlock_t *lock);
int my_spinlock_unlock(my_spinlock_t *lock);

int my_spinlock_lockTAS(my_spinlock_t *lock);
int my_spinlock_lockTTAS(my_spinlock_t *lock);
int my_spinlock_trylock(my_spinlock_t *lock);


/* queuelock starts here */

struct my_queuelock_struct {
	// fill me in
};

typedef struct my_queuelock my_queuelock_t;

int my_queuelock_init(my_queuelock_t *lock);
int myqueuelock_destory(my_queuelock_t *lock);
int my_queuelock_unlock(my_queuelock_t *lock);

int my_queuelock_lock(my_queuelock_t *lock);
int my_queuelock_trylock(my_queuelock_t *lock);


#endif
