#include"io.c"

typedef struct proc{
        struct proc *next;
               int  ksp;
}PROC;

PROC mainProc, *running;
int  procSize = sizeof(PROC);



// Print string
//prints(s) char* s; 
//{
	//while(*s != NULL)
	//{
		//putc(*s);  
		//s++; 
	//}
//}

main()
{
  running = &mainProc;
  prints("call tswitch()\n\r");
     tswitch();
  prints("back to main()\n\r");
  printf("hello %d\n"); 
}

int scheduler()
{ 
   prints("in scheduler\n\r");
}
