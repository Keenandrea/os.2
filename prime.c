#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>

#include "prime.h"

smtime* smseg;
int isetnanos; 

/*
struct sigaction sa;

void timesup(int signo, siginfo_t* siginfo, void *sigptr)
{
	char *sigmsg = "\nprime: timer exceeded 1ms. killing child process\n";
	int msglen = sizeof(sigmsg);
	write(STDERR_FILENO, sigmsg, msglen);

	shmdt(smseg);
	exit(EXIT_SUCCESS);		
}
*/

void checkprime(int pnum, int lnum)
{
	int checker, flag = 0;
	//int arriter = lnum - 1;
	for(checker = 2; checker <= pnum / 2; ++checker)
	{
		if(pnum % checker == 0)
		{
			flag = 1;
			break;
		}
	}

	/* if prime*/
	if(flag == 0)
	{
		smseg->smprime[lnum] = pnum;
		//y[arriter] = pnum;			
	}
	/*not prime*/
	if(flag == 1)
	{
		smseg->smprime[lnum] = -pnum;
		//n[arriter] = -pnum;
	}
}

int main(int argc, char *argv[])
{
	/*
	sa.sa_sigaction = timesup;
	sa.sa_flags = 0;
	if(sigaction(SIGTERM, &sa, NULL) == -1)
	{
		perror("\nprime: error: SIGQUIT");
	}
	*/	

	int segid;
	unsigned int *nans;
	key_t smkey = ftok("shareiscare", 'a');
	if(smkey == -1)
	{
		perror("\nprime: error: ftok failed");
		exit(EXIT_FAILURE);
	}
	
	segid = shmget(smkey, sizeof(smtime), 0600);
	if(segid == -1)
	{
		perror("\nprime: error: failed to create shared memory");
		exit(EXIT_FAILURE);
	}	

	smseg = (smtime*)shmat(segid,(void*)0,0);

	if(smseg == (void*)-1)
	{
		perror("\nprime: error: failed to attach shared memory");
		exit(EXIT_FAILURE);
	}

	char prime[20];
	char index[20];
	char onwrd[20];
	char mpids[20];
	char ofile[50];
	strcpy(prime, argv[1]);
	strcpy(index, argv[2]);
	strcpy(onwrd, argv[3]);
	strcpy(mpids, argv[4]);
	strcpy(ofile, argv[5]);
	int pnum = atoi(prime);
	int lnum = atoi(index);
	int inum = atoi(onwrd);
	int mnum = atoi(mpids);
	
	FILE* out = fopen(ofile, "a");
	
	fprintf(out, "\nchild process no.%i launched", lnum);
	fprintf(out, "\nclocked seconds: %i", smseg->secs);
	fprintf(out, "\nclocked nanoseconds: %u\n", smseg->nans);		
	checkprime(pnum, lnum);

	int k;
	int j;
	int y;
	int n;
	if(lnum == mnum)
	{	
		for(k = 0; k < lnum; k++)
		{
			j = smseg->smprime[k];
			if(j < 0)
			{
				smseg->n[k] = j; 
			}
			if(j > 0)
			{
				smseg->y[k] = j;
			}	 	
		}
		fprintf(out, "\nprime numbers: ");
		for(k = 0; k < lnum; k++)
		{	
			if(smseg->y[k] > 0)
			{	
				fprintf(out, " %i,", smseg->y[k]);
			}
		}
		fprintf(out, "\nother numbers: ");
		for(k = 0; k < lnum; k++)
		{	
			if(smseg->n[k] < 0)
			{	
				fprintf(out, " %i,", smseg->n[k]);
			}
		}
		fprintf(out, "\n\n");
	}

shmdt(smseg);
fclose(out);
return 0;	
}
