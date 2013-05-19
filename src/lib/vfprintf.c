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
	int  cnt=0;
	char ch;
	char *pbuf;
	while(1){
		while((ch=*format++)!='%'){
			if(ch=='\0')
			      return cnt;
			putchar_func(ch);
			cnt++;
		}
		switch ((ch=*format++)){
			case 'c':
				putchar_func(*(char *)data++);
				cnt++;
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
				for (pbuf="unsported printing format,ignore! \n"; *pbuf; pbuf ++)
				      putchar_func(*pbuf);
				break;
		}
	}
}
