/************ t.c file **********************************/
#define NPROC     9        
#define SSIZE  1024                /* kstack int size */

#define DEAD      0                /* proc status     */
#define READY     1     
#define FREE 2 

typedef struct proc
{
	struct proc *next;  
	int  ksp;               /* saved sp; offset = 2 */
	int  pid;
	int  ppid; 
	int  status;            /* READY|DEAD, etc */
	int  kstack[SSIZE];     // kmode stack of task
	int priority; 		// priority 
}PROC;

#include "io.c"  /* <===== use YOUR OWN io.c with printf() ****/



PROC proc[NPROC], *running;	// proc = array of PROC structures
PROC *readyQueue; 		// Points to ??
PROC *freeList; 

int  procSize = sizeof(PROC);

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

int body();  
int kfork(); 

// Enter p into queue
void enqueue(p,queue) PROC *p; PROC **queue;
{
	p->next = &queue; 
	
}

// Pop the first proc from queue
void dequeue(queue) PROC **queue; 
{

}

void printQueue(queue) PROC *queue; 
{

}

// Get a free proc from the freeList or 0 if none are available
PROC* getproc()
{
	PROC *p; 
	int i;
	 
	p = freeList; 

	while(p != NULL)	// iterate through freeL
	{
		if(p->status == READY)
		{
			printf("Returning proc with pid = %d\n", p->pid); 
			return p; 	// return ready process 
		}
		p = p->next; 
		

	}

	printf("No free procs found, returning 0..."); 
	return 0; 
}

int kfork()
{
	// Create a child proc and enter it into the readyQueue. 	
		// get a free proc from readyProc 
	 
	PROC *p; 

	int i,j; 
	printf("\nForking child off proc %d\n", running->pid);
	p = getproc(); 

	if(p == 0)
		return -1; 

	// Initialize p's ppid
	p->ppid = running->pid; 
	// Initialize p's kstack[] as if it called tswitch() before
	for (j=1; j<10; j++)			// To use pusha, change 10 to 11 here. 
	{
		p->kstack[SSIZE - j] = 0;          // all saved registers = 0
		p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
		p->ksp = &(p->kstack[SSIZE-9]);        // ksp -> kstack top
	}	

	// TODO: Enter p into readyQueue by priority 
	readyQueue->next = p; 
	p->status = READY;  
	freeList = freeList->next; 

	printf("Returning pid = %d\n", p->pid); 
	return p->pid; 
}

int initialize()
{
  	int i, j;
  	PROC *p;

	// Initialize proc0 as running process
	running = &proc[0]; 
	running->pid = 0;
	running->priority = 0; 
	running->status = READY;

  	for (i=1; i < NPROC; i++)
	{
    		p = &proc[i];
    		p->next = &proc[i+1];
    		p->pid = i;
    		p->status = READY;
		p->priority = 1; 
		
    
    		if (i)
		{     // initialize kstack[ ] of proc[1] to proc[N-1]
      			for (j=1; j<10; j++)			// To use pusha, change 10 to 11 here. 
			{
          			p->kstack[SSIZE - j] = 0;          // all saved registers = 0
      				p->kstack[SSIZE-1]=(int)body;          // called tswitch() from body
      				p->ksp = &(p->kstack[SSIZE-9]);        // ksp -> kstack top
			}
	// To use pusha, change 9 to 10 on the line above
    		}
  	}
	
	p->next = NULL; 	// freeList -> proc[1] -> proc[2] -> ... -> proc[N-1] -> NULL

  	running = &proc[0];
  	proc[NPROC-1].next = NULL; // &proc[1];
	freeList = &proc[1]; 
  	printf("initialization complete\n"); 
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
  running->status = DEAD;

  tswitch();   /* journey of no return */        
}

int ps()
{
  	PROC *p;

  	printf("running = %d\n", running->pid);

  	p = running;
  	p = p->next;
  	printf("readyProcs = ");
  	while(p != running && p->status==READY)
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
      		printf("Input a char : [s|q|f] ");
       		c=getc();
       		switch(c)
		{
            		case 's': tswitch(); break;
            		case 'q': grave();   break;
			case 'f': kfork(); break; 
            		default : break;  
       		}
   	}
}


main()
{
	int x;
 	printf("\nWelcome to the 460 Multitasking System\n");
   	initialize();
   	printf("P0 switch to P1\n");
	x = kfork(); 

   	tswitch();

 	printf("P0 resumes: all dead, happy ending\n");
}


int scheduler()
{
    PROC *p;
    p = running->next;

    while (p->status != READY && p != running)
      p = p->next;

    if (p == running)
       running = &proc[0];
    else
       running = p;

    printf("\n-----------------------------\n");
    printf("next running proc = %d\n", running->pid);
    printf("-----------------------------\n");
}





