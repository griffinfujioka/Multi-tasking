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
 	}
	else
	{
		rprinti(x); 
	}
	putc(' '); 
}

// printf() function 
void printf(s) char* s; 
{
	// Visualize the stack, see notes. 
	
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
}

void gets(s) char* s; 
{
	 
	*s = getc(); 
	while(*s != '\r')	// while the Enter key is not pressed 
	{
		
		putc(*s); 	// echo the character 
		s++; 		// move to the next spot in the char array 
		*s = getc();  	// read another character 
	}
	
}

