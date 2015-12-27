#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <linux/sem.h>

#define N 100

int semsetid;
pthread_t p1, p2;

int a;

void P(int semid, int index) {
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
}

void V(int semid, int index) {
	struct sembuf sem;
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
}

// print
void *subp1() {
	int i=0;
	for (i=0; i<N; i++) {
		P(semsetid, 0);
		printf("a=%d\n", a);
		V(semsetid, 1);
	}
}

// add
void *subp2() {
	int i=0;
	for (i=0; i<N; i++) {
		a ++;
		V(semsetid, 0);
		P(semsetid, 1);
	}
}

int main() {
	union semun arg;
	arg.val = 0;
	
	// create sem set
	semsetid = semget(0, 2, IPC_CREAT | 0666);
	
	// inii sem set
	semctl(semsetid, 0, SETALL, arg);

	// create threads
	pthread_create(&p1, 0, subp1, 0);
	pthread_create(&p2, 0, subp2, 0);

	// wait for threads end
	pthread_join(p1, 0);
	pthread_join(p2, 0);

	// remove sem set
	semctl(semsetid, 0, IPC_RMID, arg);
	return 0;
}

