#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h> 
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#include "clock.h"
#include "prime.h"

void detacher(pid_t pids[], int cap);

void helpme();

int allowchild = 0;
int processmax = 0;
int primes = 0;
int inc = 0;

char* ofile = NULL; 
char* ofiledead = NULL;
char* ofilesiga = NULL;

int killer = 0;
int imdead = 0;

void sighandler(int sig)
{
	switch(sig)
	{
		case SIGALRM:
			printf("\ntwo second timer\n");
			fflush(stdout);			
			break;

		case SIGINT:
			printf("\nctrl-c exit\n");
			fflush(stdout);
			break;
	}
	imdead = 1;
}
/* MAIN =============================================== */
/* ==================================================== */
int main(int argc, char *argv[])
{
	/* signal alarm handling  */
	signal(SIGQUIT, sighandler);
	alarm(2);
	signal(SIGINT,  sighandler);

	int ch;
	while( (ch = getopt(argc,argv,"hi:o:n:s:b:")) != -1 )
    	{
        	switch(ch)
        	{
            		case 'h':
                	helpme();
                	return 0;
            		
			case 'n':
                	processmax = atoi(optarg);
                	break;
            
			case 's':
                	allowchild = atoi(optarg);
                	if(allowchild > 15)
                	{
                    		allowchild = 15;
                	}
                	if(allowchild > processmax)
                	{
                    		allowchild = processmax;
                	}
                	break;
            
			case 'b':
                	primes = atoi(optarg);
                	break;
            		
			case 'i':
                	inc = atoi(optarg);
                	break;
            
			case 'o':
                	ofile = optarg;
			ofiledead = ofile;
			ofilesiga = ofile;
                	break;
 
            		case '?': 
                		printf("\noss: error: lame or absent argument\n");
                		return 0;
    		}
	}
	/* defaults to 2 */
	if(allowchild == 0)
	{
		allowchild = 2;
	}
	/* defaults to 4 */
	if(processmax == 0)
	{
		processmax = 4;
	}
	if(ofile == NULL)
	{
		ofile = "output.log";
		ofiledead = "output.log";
		ofilesiga = "output.log";
	}	

	clockinit();
	
	pid_t pids[processmax], childpid, exitpid;

	int status;
	int logics = 1;
	int depart = 0;
	int active = 0;
	int onward = 0;
	int growth = 10000;
	int maxpid = processmax;
	int i;

	FILE *ofptr;
	ofptr = fopen(ofile,"a");
	if(ofptr == NULL)
	{
		perror("\noss: error: failed to open output file");
		exit(EXIT_SUCCESS);
	}
	fprintf(ofptr, "\nConcurrent UNIX Processes and Shared Memory Log\n");	
	fclose(ofptr);
	
	while(1)
	{
		clockaug(growth);
		
		if(imdead == 1)
		{
			detacher(pids,maxpid);
		}		
		
		if(processmax != 0 && active < allowchild)
		{
			active++;
			processmax--;
			childpid = fork();
			if(childpid < 0)
			{
				perror("\noss: error: fork failed");
				exit(EXIT_FAILURE);
			}
			if(childpid == 0)
			{
				char prime[20];
				char index[20];
				char onwrd[20];
				char mpids[20];	
				snprintf(prime, sizeof(prime), "%i", primes);
				snprintf(index, sizeof(index), "%i", logics);
				snprintf(onwrd, sizeof(onwrd), "%i", onward);
				snprintf(mpids, sizeof(mpids), "%i", maxpid); 
				char* fargs[] = {"./prime", prime, index, onwrd, mpids, ofile, NULL};
				execv(fargs[0], fargs);		 
			}
			
			onward++;
			primes += inc;
			pids[onward - 1] = childpid;
			
			for(i = 0; i < maxpid; i++)
			{
				exitpid = waitpid(pids[i], &status, 0);
				if(exitpid > 0)
				{
					FILE *ofptrdead = fopen(ofiledead, "a");
					if(ofptrdead == NULL)
					{
						perror("\noss: error: failed to open output file");
						break;
					}		
					fprintf(ofptrdead, "child process no.%i time of death: %i seconds and %u nanoseconds\n", logics, getsecs(), getnans());				
					pids[i] = 0;
					logics++;
					depart++;	
					active--;
					fclose(ofptrdead);
				}
			}
		}
		if(processmax == 0 || depart == maxpid)
		{
			break;
		}	
	}

clockdetach();
return 0;
}
/* DETACHER ===================================
 * ========================================= */
void detacher(pid_t pids[], int cap)
{
	int killer;
	FILE* ofptrsiga = fopen(ofilesiga, "a");
	if(ofilesiga == NULL)
	{
		perror("\noss: error: failed to open output file");
	} 
	clockdetach();
	for(killer = 0; killer < cap; killer++)
	{
		kill(pids[killer], SIGKILL);
	}
	fprintf(ofptrsiga, "parent time of death: %i seconds and %u nanoseconds\n", getsecs(), getnans());
	fclose(ofptrsiga);
	exit(EXIT_SUCCESS);
}
/* ============================================
 * ========================================= */

/* HELP MENU ==================================
 * ========================================= */
void helpme()
{
    printf("\n|INSTRUCTIONS|");
    printf("\t-h : display instructions menu\n");
    printf("\t-n <x> : indicate the maximum total of child processes oss will ever create. defaults to 4\n");
    printf("\t-s <x> : indicate the number of children allowed to exist in the system at the same time. defaults to 2\n");
    printf("\t-b <B> : start of the sequence of numbers to be tested for primality\n");
    printf("\t-i <I> : increment between numbers that we test\n");
    printf("\t-o <filename> : select output filename. defaults to output.log\n");
}
/* ============================================
 * ========================================= */
