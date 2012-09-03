typedef struct proc{
        struct proc *next;
               int  ksp;
}PROC;

PROC mainProc, *running;
int  procSize = sizeof(PROC);

prints(s) char *s;
{ YOUR prints() function }

main()
{
  running = &mainProc;
  prints("call tswitch()\n\r");
     tswitch();
  prints("back to main()\n\r");
}

int scheduler()
{ 
   prints("in scheduler\n\r");
}
