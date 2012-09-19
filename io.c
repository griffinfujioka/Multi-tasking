#include<string.h>

#define u8 unsigned char 
#define u16 unsigned short 
#define u32 unsigned long
char* table = "0123456789ABCDEF"; 

// Print string
void prints(s) char* s; 
{
	 
	while(*s != NULL)
	{
		putc(*s);  
		s++; 
	}
}

// Recursive driver for printing integers
void rprinti(x) int x; 
{ 
	char c;  
	if(x)		// x != 0 
	{
		c = table[x % 10]; 
		rprinti(x/10); 
		putc(c); 
	}
}

// Main function for printing integers 
void printi(x) int x; 
{
	if(x==0) 
	{ 
		putc('0'); 
		return; 
 	}
	rprinti(x); 
	putc(' '); 
}

// printf() function 
void printf(s) char* s; 
{
	// Visualize the stack, see notes. 
	/* Example: printf("hi there %s %d", a, b); 

	   ----------------------------------------
	   |\\\\\\| b | a | *format_string | retPC |	    <--- stack before entering printf()
	   ----------------------------------------- 

	   -------------------------------------------------
	   |\\\\\\| b | a | *format_string | retPC | FP | p <--- stack after entering printf()
	   ------------------------------------------------- 

	*/ 


	// (1) Push FP to stack - done upon entry into function 
	// (2) Use int* p for traversing stack
	// (3) Copy BP into ax register 
	// (4) Use char* c to traverse the format string
		// When you witness a %, look for a tag (i.e., '%s', '%d')
		// if(tag == 's') - Call prints()
		// else if(tag == 'd') - Call printi()
	
	char* cp; 
	int *ip; 	// u16! TAKE NOTE
	int *up; 	// u32! TAKE NOTE


	cp = s;	 	// cp starts at format string

		
	ip = (int *)&s + 1; 	// Should give the value of the parameter 'a'
	 
	
	while(*cp)
	{
		if(*cp != '%')
		{
			putc(*cp); 
			if(*cp == '\n')
			{
				putc('\r'); 
			}
			cp++; 
			continue; 
		}
	} 	// } may be in wrong spot, may need to extend past the switch statement. 
	cp++; 
	switch(*cp)
	{
		case 'c': putc(*ip); break; 
		case 's': prints(*ip); break; 
		case 'd': printi(*ip); break; 
	}
	cp++; 
	ip++;  
}


