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
	char ch;
	char *str;
	char *pbuf;
	while(1){
		while((ch=*format++)!='%'){
			if(ch=='\0')
			      return 0;
			putchar_func(ch);
		}
		switch ((ch=*format++)){
			case 'c':
				putchar_func(*(char *)data++);
				break;
			case 'd':
				pbuf=itoa(*(int *)data++,10);
				while((ch=*pbuf++)!=0)
				      putchar_func(ch);
				break;
			case 'x':
				pbuf=utoa(*(unsigned int *)data++,16);
				while((ch=*pbuf++)!=0)
				      putchar_func(ch);
				break;
			case 's':
				str=*(char **)data++;
				while((ch=*str++)!=0)
				      putchar_func(ch);
				break;
			default :
				for (str="unsported printing format!\n"; *str; str ++)
				      putchar_func(*str);
				break;
		}
	}
	return 0;
}
