#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "clock.h"

int segid = 0;
typedef struct
{
	int nans;
	int secs;
	int smprime[20];
	int y[20];
	int n[20];
} smtime;

smtime* smseg;

void clockinit()
{
	key_t smkey = ftok("shareiscare", 'a');
	if(smkey == -1)
	{
		perror("\noss: error: ftok failed");
		exit(EXIT_FAILURE);
	}

	segid = shmget(smkey, sizeof(smtime), 0600 | IPC_CREAT);
	if(segid == -1)
	{
		perror("\noss: error: failed to create shared memory");
		exit(EXIT_FAILURE);
	}

	smseg = (smtime*)shmat(segid,(void*)0,0);
	
	/* on success, shmat()
 * 	   returns the address
 * 	   of the attached sha
 * 	   red memory seg. the
 * 	   n, on error, (void*
 * 	   )-1 is returned, an
 * 	   d errno is set to i
 * 	   ndicate the error*/ 
	if(smseg == (void*)-1)
	{
		perror("\noss: error: failed to attach shared memory");
		exit(EXIT_FAILURE);
	}

	smseg->nans = 0;
	smseg->secs = 0;
}

void clockdetach()
{
	shmdt(smseg);
	shmctl(segid, IPC_RMID, NULL);
}

void clockaug(int aug)
{
	smseg->nans = smseg->nans + aug;
	if(smseg->nans % 1000000000 == 0)
	{
		smseg->secs = smseg->secs + 1;
	}
} 

unsigned int getnans()
{
	return smseg->nans;
}

unsigned int getsecs()
{
	return smseg->secs;
}
