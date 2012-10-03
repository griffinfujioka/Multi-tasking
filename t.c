/************ t.c file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

#define DEAD   	0                /* proc status     */
#define READY   1     
#define FREE 	2 
#define SLEEP 	3	
#define ZOMBIE 	4
#define PROC0 	5 

typedef struct proc
{
	struct proc *next;  
	int  ksp;               /* saved sp; offset = 2; used for switching processes */
	int  pid;		/* proc ID */ 
	int  ppid; 		/* parent's ID */ 
	int  status;            /* READY|DEAD*/
	int  kstack[SSIZE];     /* process stack */ 
	int priority; 		/* priority */ 
	int event; 		/* event */ 
	int exit; 		/* exit value */ 
}PROC;


#include "io.c"  	/* Includes my implementation of some I/O functions */ 
#include <stdlib.h> 	/* for atoi */ 

PROC proc[NPROC];		// array of PROC structures
PROC *running; 		// currently running PROC
PROC *freeList, *readyQueue; 		// Point to first available free PROC in proc array 

int  procSize = sizeof(PROC);
int debug = 1; 

/****************************************************************
 Initialize the proc's as shown:
        running ---> proc[0] -> proc[1];

        proc[1] to proc[N-1] form a circular list:

        proc[1] --> proc[2] ... --> proc[NPROC-1] -->
          ^                                         |
          |<---------------------------------------<-

        Each proc's kstack contains:
        retPC, ax, bx, cx, dx, bp, si, di, flag;  all 2 bytes
*****************************************************************/

int body();  			/* Well, I don't enjoy this ugly nonsense... */ 
int kfork(); 
void printReadyQueue();
void sleep(); 
void wakeup(); 
int wait(); 
void kexit(); 

// Enter p into queue by priority 
void enqueue(p,queue) PROC *p; PROC **queue;
{
	PROC *current, *prev; 
	
	if(debug)
	{
		if(*queue != NULL)
		{
			printf("\n-- ENQUEUE P%d to P%d\n ", p->pid, readyQueue->pid);
		}
		else
			printf("\n-- ENQUEUE P%d to Empty Queue\n ", p->pid);
	}

	if((*queue) == NULL) 	// insert into empty queue 
	{
		printf("queue is empty, adding P%d\n", p->pid); 
		p->next = (*queue); 
		(*queue) = p; 
		readyQueue = p; 	// Hack
		printf("readyQueue = P%d\n", readyQueue->pid); 
	}
	else
	{ 	
		current = (*queue);
		while((current) && (p->priority <= current->priority)) 
		{
			if(debug)
				printf(".");  // Tells you how far you went through readyQueue before you finally inserted

			prev = current; 
			current = current->next; 

		}
		
		p->next = current; 
		prev->next = p; 
		
	}
	
	printf("\n"); 
}
// Pop the first proc from queue
PROC* dequeue(queue) PROC **queue; 
{
	PROC *p; 
	p = (*queue);

	if (*queue != NULL)
	{
		*queue = (*queue)->next;
	}
	else
	{
		if(debug)
			printf("dequeue returning NULL \n"); 
	}


	return p;
}

void printQueue(queue) PROC *queue; 
{
	PROC *p; 

	printf("running = %d\n", running->pid);

  	p = queue; 
	while(p != NULL)
	{
    		printf("%d -> ", p->pid);
    		p = p->next;
	}
  	printf("\n");
}


// Return a free proc from the freeList or 0 if none are available
PROC* getproc()
{
	return (dequeue(&freeList)); 

}

// get a free proc from freeList and enter it into the readyQueue.
int kfork()
{
	 	
	PROC *p; 
	int j; 
	int index; 
	printf("Forking child off P%d\n", running->pid);
	p = getproc(); 	// get free PROC from freeList

	if(p == 0) // No free PROCs found
		return -1; 

	// Initialize the new proc
	p->status = READY;
	p->ppid = running->pid; 
	p->priority = 1; 
	p->next = 0; 	

	
	// Zero out all of the PROC's registers 
	for (j=1; j<10; j++)			// To use pusha, change 10 to 11 here. 
	{
		p->kstack[SSIZE - j] = 0;          
		
	}
	p->kstack[SSIZE-1] = (int)body;          // Call tswitch() from body, fill retPC register with return value 
	p->ksp = &(p->kstack[SSIZE-9]);        	// ksp -> top of kstack

	
	if(debug)
		printf("Calling enqueue from kfork() to insert P%d into readyQueue\n", p->pid); 

	enqueue(p, (&readyQueue)); 	// insert p into readyQueue by priority 
	
	if(debug)
		printf("kfork forked P%d off of parent P%d\n", p->pid, p->ppid); 

	return (p->pid); 
}


int initialize()
{
  	int i, j;
  	PROC *p;

	printf("Initializing..."); 

	// Initialize proc array with all PROCs free
	for(i=0; i<NPROC; i++)
	{
		proc[i].pid = i; 
		proc[i].status = FREE; 
		proc[i].next = &proc[i+1]; 
		proc[i].kstack[SSIZE - 1] = (int)body; 	// Save PROCs return address in retPC register
		p->ksp = &(p->kstack[SSIZE-9]);        // ksp = top of kstack
		
		
	}
	proc[NPROC-1].next = NULL;  	// Not a circular linked-list
	freeList = &proc[0]; 		/* Initially, all PROCs are free 	*/  
	readyQueue = 0; 		/* and readyQueue is empty		*/ 
	
	// Create P0
	p = getproc();  	// Get P0 from freeList
	p->status = PROC0; 	// P0 has a status = PROC0, it's a special case AKA I hacked this part. 
	p->priority = 0; 	// P0 has a priority of 0 
	p->pid = 0; 		// P0 has a pid of 0 
	p->next = &proc[1]; 	// P0.next = P1
	p->ppid = running->pid;  // P0 is it's own parent 	
	running = p; 		// Initially P0 is running 

  	for(i=1; i<NPROC; i++)
	{
		proc[i].priority = 1; 	// All other PROCs have a priority of 1
	}
  	printf(" complete.\n"); 
}

char *gasp[NPROC]={
     "Oh! You are killing me .......",
     "Oh! I am dying ...............", 
     "Oh! I am a goner .............", 
     "Bye! Bye! World...............",      
};

int grave(){
  printf("\n*****************************************\n"); 
  printf("Task %d %s\n", running->pid,gasp[(running->pid) % 4]);
  printf("*****************************************\n");

  running->status = FREE;
  enqueue(running, &freeList); 	// Put running back in the freeList

  tswitch();   /* journey of no return */        
}

int ps()
{
  	PROC *p;

	int i; 

  	// This is pretty crappy/hacky code... could use some cleaning up. 
	printf("Running P%d in ps()\n", running->pid); 
  	printf("ready procs = ");
  	for(i=1; i<NPROC; i++)
	{
		
		p = &proc[i]; 
		if(p->status == READY)
		{
			 
			printf("%d -> ", p->pid);
		}
    		 
 	}

	if(debug)	// Trying to debug readyQueue 
	{
		p = running; 
		printf("\nreadyQueue= "); 
		while(p)
		{
			printf("%d -> ", p->pid); 
			p = p->next; 
		}
		
	}
	 
	p = freeList; 
	printf("\nfree list = "); 
	while(p && p->status == FREE)
	{
		printf("%d -> ", p->pid); 
		p = p->next; 
	}

  	printf("\n");
}

int body()
{  
	char c;
   	while(1)
	{
      		ps();	// print all procs 
      		printf("I am Proc %d in body()\n", running->pid);
      		printf("Input a char : [s|q|f|z|k|w|p] ");
       		c=getc();
       		switch(c)
		{
            		case 's': tswitch(); break;
            		case 'q': grave();   break; 	/* QUIT */ 
			case 'f': kfork(); break; 	/* FORK */ 
			case 'z': sleep(running->pid); break; 	/* SLEEP */ 
			case 'k': wakeup(running->event); break; /* WAKE -- NOT COMPLETE/CORRECT */ 
			case 'w': break; 		/* WAIT */ 
			case 'p': ps(); break; 	/* PRINT ---> Break here? */ 
			
            		default : break;  
       		}
   	}
}


main()
{
	
 	printf("\nWelcome to the 460 Multitasking System\n");
   	initialize();
   	printf("P0 forks P1\n");
	kfork(); 	// fork P1 
	printf("P0 switches to P1... calling tswitch()\n"); 
   	tswitch();	// switches running to P1

	// Switch, Quit & Fork processes until all of them are dead except P0
	printf("Almost done... running is P%d\n", running->pid); 
 	printf("P0 resumes: all dead, happy ending\n");
}


int scheduler()
{
	PROC *p;
 	

	if(running->status == READY)
	{
		enqueue(running, (&readyQueue));
	} 

	
	running = dequeue(&readyQueue);  
	
	

	printf("Running P%d in scheduler()\n", running->pid); 
	 
}

/**********************************************************
	Lab 3 Functions:
		Sleep, Wakeup, Wait, kexit
***********************************************************/ 
void sleep(event) int event;
{
	running->event = event; 	/* Record event in PROC structure */ 
	running->status = SLEEP; 	/* Adjust PROC status accordingly */ 
	tswitch(); 			/* Remove from readyQueue 	*/ 
}

void wakeup(event) int event; 
{
	PROC* p; 
	int i=0; 

	p = proc; 	// Start at beginning of proc[NPROC] array
	while(i<NPROC)
	{
		if(proc[i].event == event)	// Wake that sucker up!
		{
			proc[i].status = READY; 
			enqueue(&proc[i], &readyQueue); 	// Put PROC back in readyQueue. TODO: Check if correct parameter usage.... 
		}
		i++; 		
	}
}

int wait(status) int *status; 
{
	int i=0; 

	/* Find child */ 
	while((proc[i].ppid == running->pid) && i<NPROC)
		i++; 

	if(i >= NPROC)		/* Went through all PROCs, didn't find child */ 
	{
		if(debug)
			printf("Could not find child\n"); 

		return -1; 
	}

	while(1)
	{
		for(i=0; i<NPROC; i++)
		{
			if((proc[i].ppid == running->pid) && (proc[i].status == ZOMBIE))
			{	/* Found zombie child! */ 
				status = &proc[i].exit; 	/* TODO: Type checking. proc.exit is int, status is int* */ 
				proc[i].status = FREE; 
				enqueue(&proc[i], &freeList); 	/* free the Zombie child PROC */ 
				
				return proc[i].pid; 
			}
		}
	}
	sleep(running->pid); /* Sleep at it's own &PROC */ 
}


/* A running PROC will call kexit(value) to die */ 
void kexit(exitValue) int exitValue; 
{
	char* buff; 	/* Temporary buffer for getting user input */ 
	int i; 

	printf("\nEnter an exit value: "); 		/* Prompt for exit value */ 
	gets(buff); 	/* Read user input into buff */ 
	/* TODO: Ensure that user inputted exit value is between 0-9 */ 
	exitValue = atoi(buff); 	/* Store integer exit value in exitValue */ 
	if(debug)
	{
		printf("\nUser entered %s\n", buff); 
		printf("\nInteger representation is %d", exitValue); 
	}

	
	/* Print last gasp */ 
	printf("\n*****************************************\n"); 
  	printf("Task %d %s\n", running->pid,gasp[(running->pid) % 4]);
  	printf("*****************************************\n");

	running->exit = exitValue; 
	running->status = ZOMBIE; 	/* ZOMBIE status */ 
	
	/* Give all children to P1 */ 
	for(i=0; i<NPROC; i++)
	{
		if(proc[i].ppid == running->pid)
			proc[i].ppid = 1; 
	}

	wakeup(running->ppid); 	/* Wake up it's parent */ 
	tswitch(); 		/* Give up CPU resources */ 
		
	
}








