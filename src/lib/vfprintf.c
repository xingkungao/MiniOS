#include "string.h"
#include "assert.h"
#include "common.h"

/*
 * use putchar_func to print a string
 *   @putchar_func is a function pointer to print a character
 *   @format is the control format string (e.g. "%d + %d = %d")
 *   @data is the address of the first variable argument
 * please implement it.
 */
int
vfprintf(void (*putchar_func)(char), const char *format, void **data) {
	int  cnt=0;					//number of the printed char
	char ch;
	char *pbuf;
	while(1){
		while((ch=*format++)!='%'){
			if(ch=='\0'){
			      return cnt;
			}
			putchar_func(ch);
			cnt++;
		}
		switch ((ch=*format++)){
			case 'c':
				putchar_func(*(char *)data++);      	//Notice data=data+1, data proceeds 4 bytes,
				cnt++;			      		//beacause of 4 bytes of alignment.
				break;
			case 'd':
				pbuf=itoa(*(int *)data++,10);
				while((ch=*pbuf++)!=0){
					putchar_func(ch);
					cnt++;
				}
				break;
			case 'x':
				pbuf=utoa(*(unsigned int *)data++,16);
				while((ch=*pbuf++)!=0){
					putchar_func(ch);
					cnt++;
				}
				break;
			case 's':
				pbuf=*(char **)data++;
				while((ch=*pbuf++)!=0){
					putchar_func(ch);
					cnt++;
				}
				break;
			default :

			// If this foramt specifier is invalid, print it out
			// with '%' as const string. Specifically, if it's '\0' which 
			// ends the whole format string, the pointer format needs to
			// go back, thus, we can return to the while loop safely, or unexpected
			// behavior happens
			

				putchar_func('%');			
				cnt++;
				if(ch!='\0'){
			      	putchar_func(ch);	
				cnt++;
				}
				else  format--;
		}
	}
}
