#include <stdio.h>
#include <stdarg.h>

void itoa(unsigned long val,char *buf,unsigned radix)
{
	char *p;/*pointer to traverse string*/
	char *firstdig;/*pointer to first digit*/
	char temp;/*temp char*/
	unsigned digval;/*value of digit*/
 
	p = buf;
	firstdig = p;/*save pointer to first digit*/
 
	do {
		digval =(unsigned)(val % radix);
		val /= radix;/*get next digit*/
 
		/*convert to ascii and store */
		if(digval > 9)
			*p++ = (char)(digval - 10 + 'a');/*a letter*/
		else 
			*p++ = (char)(digval + '0');/*a digit*/
	} while(val > 0);
 
		/*We now have the digit of the number in the buffer,but in reverse
		order.Thus we reverse them now.*/
 
	*p-- = '\0';/*terminate string;p points to last digit*/
 
	do {
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;/*swap *p and *firstdig*/
		--p;
		++firstdig;/*advance to next two digits*/
	} while(firstdig < p);/*repeat until halfway*/
}

/*putc is change to uart_putc*/
int print( const char *format, ...)
{
	va_list arg;
	int done = 0;

	va_start (arg, format); 

	while ( *format != '\0') {
		if ( *format == '%') {
			if ( *(format+1) == 'c' ) {
				char c = (char)va_arg(arg, int);
				putc(c, stdout);
			} else if( *(format+1) == 'd' || *(format+1) == 'i') {
				char store[20];
				int i = va_arg(arg, int);
				char* str = store;
				itoa(i, store, 10);
				while( *str != '\0') putc(*str++, stdout); 
			} else if( *(format+1) == 'o') {
				char store[20];
				int i = va_arg(arg, int);
				char* str = store;
				itoa(i, store, 8);
				while( *str != '\0') putc(*str++, stdout); 
			} else if( *(format+1) == 'x') {
				char store[20];
				int i = va_arg(arg, int);
				char* str = store;
				itoa(i, store, 16);
				while( *str != '\0') putc(*str++, stdout); 
			} else if( *(format+1) == 's' ) {
				char* str = va_arg(arg, char*);
				while( *str != '\0') putc(*str++, stdout);
			}
			format += 2;
		} else {
			putc(*format++, stdout);
		}
	}

	va_end (arg);

	return done;
}
