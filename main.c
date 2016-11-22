#include "main.h"


void *fnc()
{
	int i;
	for(i = 0; i < 1000000; i++)
	{
		c++;
	}
}


pthread_mutex_t count_mutex;
void *pthreadMutexTest()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	for(i = 0; i < numItterations; i++)
	{
		for(j = 0; j < workOutsideCS; j++)
		{
			localCount++;
		}

		pthread_mutex_lock(&count_mutex);
		for(k = 0; k < workInsideCS; k++)
		{
			c++;
		}
		pthread_mutex_unlock(&count_mutex);
	}
}

pthread_spinlock_t count_spin;
void *pthreadSpinTest()
{
	int i;
	int j;
	int k;

	int localCount = 0;

	for(i = 0; i < numItterations; i++)
	{

		for(j = 0; j < workOutsideCS; j++)
		{
			localCount++;
		}

		pthread_spin_lock(&count_spin);
		//if(pthread_spin_lock(&count_spin)==0){
			for(k = 0; k < workInsideCS; k++)
			{
				c++;
			}
		//}
		pthread_spin_unlock(&count_spin);
	}
}

my_spinlock_t mySpin;
void *mySpinLockTAS()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	for(i = 0; i < numItterations; i++)
	{
		for(j = 0; j < workOutsideCS; j++)
		{
			localCount++;
		}

		my_spinlock_lockTAS(&mySpin);
		for(k = 0; k < workInsideCS; k++)
		{
			c++;
		}
		my_spinlock_unlock(&mySpin);
	}
}

void *mySpinLockTTAS()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	for(i = 0; i < numItterations; i++)
	{
		for(j = 0; j < workOutsideCS; j++)
		{
			localCount++;
		}

		my_spinlock_lockTTAS(&mySpin);
		for(k = 0; k < workInsideCS; k++)
		{
			c++;
		}
		my_spinlock_unlock(&mySpin);
	}
}

my_mutex_t myMutex;
void *myMutexTAS()
{
	int i;
	int j;
	int k;
	int localCount = 0;
	for(i = 0; i < numItterations; i++)
	{
		for(j = 0; j < workOutsideCS; j++)
		{
			localCount++;
		}

		my_mutex_lock(&myMutex);
		for(k = 0; k < workInsideCS; k++)
		{
			c++;
		}
		my_mutex_unlock(&myMutex);
	}
}

// my_queuelock_t myQueue;
// void *myQueueTAS()
// {
// 	int i;
// 	int j;
// 	int k;
// 	int localCount = 0;
// 	for(i = 0; i < numItterations; i++)
// 	{
// 		for(j = 0; j < workOutsideCS; j++)
// 		{
// 			localCount++;
// 		}
//
// 		my_queuelock_lock(&myQueue);
// 		for(k = 0; k < workInsideCS; k++)
// 		{
// 			c++;
// 		}
// 		my_queuelock_unlock(&myQueue);
// 	}
// }

void* multilock() {
	my_spinlock_lockTAS(&mySpin);
	printf("lock\n");
	my_spinlock_lockTAS(&mySpin);
	printf("lock\n");
	my_spinlock_lockTAS(&mySpin);
	printf("lock\n");
	my_spinlock_lockTAS(&mySpin);
	printf("lock\n");
	my_spinlock_lockTAS(&mySpin);
	printf("lock\n");

	my_spinlock_unlock(&mySpin);
	printf("unlock\n");
	my_spinlock_unlock(&mySpin);
	printf("unlock\n");
	my_spinlock_unlock(&mySpin);
	printf("unlock\n");
	my_spinlock_unlock(&mySpin);
	printf("unlock\n");
	my_spinlock_unlock(&mySpin);
	printf("unlock\n");
}

int runTest(int testID)
{

// you must create all data structures and threads for experiments

if (testID == 0 || testID == 1) /*pthread mutex */
{
	c = 0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit int

	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for( i = 0; i < numThreads; i++)
	{

		if( rt=(pthread_create(threads+i, NULL, &pthreadMutexTest, NULL)) )
		{
			printf("Thread creation failed: %d\n", rt);
			return -1;
		}

	}

	for( i = 0; i < numThreads; i++) // wait for all threads to finish
	{
		pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run Pthread (Mutex) Total Count: %llu\n", c);
	result = timespecDiff(&stop, &start);
	printf("Pthread Mutex time(ms): %lld\n", result/1000000);

}

if(testID == 0 || testID == 2) /*Pthread SPinlock */
{
	//pthread spinlock goes here
	pthread_spin_init(&count_spin, 0);
	//printf("Starting spinlock\n");
	c = 0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit int

	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for( i = 0; i < numThreads; i++)
	{

		if( rt=(pthread_create(threads+i, NULL, &pthreadSpinTest, NULL)) )
		{
			printf("Thread creation failed: %d\n", rt);
			return -1;
		}

	}
	for( i = 0; i < numThreads; i++) // wait for all threads to finish
	{
		pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run Pthread (Spin) Total Count: %llu\n", c);
	result = timespecDiff(&stop, &start);
	printf("Pthread Spin time(ms): %lld\n", result/1000000);
}

if(testID == 0 || testID == 3) /*MySpinlockTAS*/
{
	//myspinlock TAS goes here
	my_spinlock_init(&mySpin);
	c = 0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit int

	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for( i = 0; i < numThreads; i++)
	{
		if( rt=(pthread_create(threads+i, NULL, &mySpinLockTAS, NULL)) )
		{
			printf("Thread creation failed: %d\n", rt);
			return -1;
		}
	}
	for( i = 0; i < numThreads; i++) // wait for all threads to finish
	{
		pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run mySpinLockTAS  (Spin) Total Count: %llu\n", c);
	result = timespecDiff(&stop, &start);
	printf("mySpinLockTAS time(ms): %lld\n", result/1000000);
}

if(testID == 0 || testID == 4) /*MySpinLockTTAS*/
{
	//myspinlock TTAS
	my_spinlock_init(&mySpin);
	//printf("Starting spinlock\n");
	c = 0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit int

	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for( i = 0; i < numThreads; i++)
	{

		if( rt=(pthread_create(threads+i, NULL, &mySpinLockTTAS, NULL)) )
		{
			printf("Thread creation failed: %d\n", rt);
			return -1;
		}

	}
	for( i = 0; i < numThreads; i++) // wait for all threads to finish
	{
		pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run mySpinLockTTAS  (Spin) Total Count: %llu\n", c);
	result = timespecDiff(&stop, &start);
	printf("mySpinLockTTAS time(ms): %lld\n", result/1000000);
}

if(testID == 0 || testID == 5) /*MyMutexTAS*/
{
	//myMutex TAS
	my_mutex_init(&myMutex);

	c = 0;
	struct timespec start;
	struct timespec stop;
	unsigned long long result; //64 bit int

	pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	int i;
	int rt;

	clock_gettime(CLOCK_MONOTONIC, &start);
	for( i = 0; i < numThreads; i++)
	{

		if( rt=(pthread_create(threads+i, NULL, &myMutexTAS, NULL)) )
		{
			printf("Thread creation failed: %d\n", rt);
			return -1;
		}

	}
	for( i = 0; i < numThreads; i++) // wait for all threads to finish
	{
		pthread_join(threads[i], NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &stop);

	printf("Threaded Run myMutexTAS  (Spin) Total Count: %llu\n", c);
	result = timespecDiff(&stop, &start);
	printf("myMutexTAS time(ms): %lld\n", result/1000000);
}

if(testID == 0 || testID == 6) /*MyQueueLock*/
{
	//myqueuelock
	// my_queuelock_init(&myQueue);
	//
	// c = 0;
	// struct timespec start;
	// struct timespec stop;
	// unsigned long long result; //64 bit int
	//
	// pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);
	// int i;
	// int rt;
	//
	// clock_gettime(CLOCK_MONOTONIC, &start);
	// for( i = 0; i < numThreads; i++)
	// {
	//
	// 	if( rt=(pthread_create(threads+i, NULL, &myQueueTAS, NULL)) )
	// 	{
	// 		printf("Thread creation failed: %d\n", rt);
	// 		return -1;
	// 	}
	//
	// }
	// for( i = 0; i < numThreads; i++) // wait for all threads to finish
	// {
	// 	pthread_join(threads[i], NULL);
	// }
	// clock_gettime(CLOCK_MONOTONIC, &stop);
	//
	// printf("Threaded Run myQueueTAS  (Spin) Total Count: %llu\n", c);
	// result = timespecDiff(&stop, &start);
	// printf("myQueueTAS time(ms): %lld\n", result/1000000);
}

	return 0;
}

int testAndSetExample()
{
	volatile long test = 0; //test is set to 0
	printf("Test before atomic OP:%ld\n", test);
	tas(&test);
	printf("Test after atomic OP:%ld\n", test);
}

int processInput(int argc, char *argv[])
{

/*testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock*/
	/*You must write how to parse input from the command line here, your software should default to the values given below if no input is given*/

	if ( argc > 2) {
		int i = 0;
		for (i = 1; i < argc; i+=2) {
			if(strcmp(argv[i],"-t")== 0){
				numThreads = atoi(argv[i+1]);
				printf("numThreads set to: %d\n", numThreads);
			} else if(strcmp(argv[i],"-i")== 0){
				numItterations = atoi(argv[i+1]);
				printf("numItterations set to: %d\n", numItterations);
			} else if(strcmp(argv[i],"-o")== 0){
				workOutsideCS = atoi(argv[i+1]);
				printf("workOutsideCS set to: %d\n", workOutsideCS);
			} else if(strcmp(argv[i],"-c")== 0){
				workInsideCS = atoi(argv[i+1]);
				printf("workInsideCS set to: %d\n", workInsideCS);
			} else if(strcmp(argv[i],"-d")== 0){
				testID = atoi(argv[i+1]);
				printf("testID set to: %d\n", testID);
			}
		}
	} else {
		numThreads=4;
		numItterations=1000000;
		testID=0;
		workOutsideCS=0;
		workInsideCS=1;
	}

	return 0;
}


int main(int argc, char *argv[])
{


	printf("Usage of: %s -t #threads -i #Itterations -o #OperationsOutsideCS -c #OperationsInsideCS -d testid\n", argv[0]);
	printf("testid: 0=all, 1=pthreadMutex, 2=pthreadSpinlock, 3=mySpinLockTAS, 4=mySpinLockTTAS, 5=myMutexTAS, 6=myQueueLock, \n");

	// testAndSetExample(); //Uncomment this line to see how to use TAS

	processInput(argc,argv);
	runTest(testID);
	return 0;

}
