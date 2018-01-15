#include <sys/kprintf.h>
#include <stdarg.h>
#define bufferSize 1000
#include <sys/defs.h>
#include <sys/string.h>


int x = 0, y = 0;
volatile char* vPointer = (volatile char*)(0xffffffff800b8000);

char * reverseString(char *dArgList);
void reverse(char *p);
void printInt(int num);

void printChar(char c) {
     //vPointer = (long*)(0xffffffff800b8000) + (y * 80 + x) ;
     //*vPointer = c | (0x0F << 8);
     *vPointer++ = c;
     *vPointer++ = 7;
     //shift x coordinate after printing character
     x++;
} 

/*void doScroll(){
   	char c;
	char* tempPointer = (char*)(0xffffffff800b8000);
	for(int i=0;i<1600;i++){
		if(x == 80) {
                	x = 0;
                	y++;
		}
		tempPointer = (char*)(0xffffffff800b8000) + ((y+1) * 80 + x) ;
		c = *tempPointer;
    		//vPointer = (long*)(0xffffffff800b8000) + (y * 80 + x) ;
    		// *vPointer = c | (0x0F << 8);
		*vPointer++ = c;
	        *vPointer++ = 7;

		*tempPointer = '\0';
    		//shift x coordinate after printing character
    		x++;
	}
}*/
void doScroll() {
	uint64_t tempV=(uint64_t)((char*)0xffffffff800B8000);
	y=22;
   	memcpy((void*)tempV,(void*)tempV+160,23*160);
	unsigned char *p = (void*)(tempV+23*160);
	int value = 0;
	int length = 160;
	while(length>0) {
                *p = value;
                p++;
                length--;
        }	
   	vPointer=((char*)0xffffffff800B8000)+y*160;
}

void clearScreen(){
	uint64_t tempV=(uint64_t)((char*)0xffffffff800B8000);
	memset1((void*)(tempV),0,23*160);
	x = 0; y = 0;
	vPointer = (volatile char*)(0xffffffff800b8000);
}

void clearChar() {
	if(x>0) {
		x--;
	} else {
		x = 80;
		y--;
	}
	
	(*--vPointer) = 0;
     	(*--vPointer) = 0;
}

void printPointer(unsigned long  pArg){
	int rem=0, rem1=0;
        int i=0;
        rem = pArg % 16;
        pArg = pArg/16;
        if(pArg > 0) {
                printPointer(pArg);
        }
        else if(pArg == 0){
                if((rem / 10) < 1){
        	        rem1 = rem % 10;
                	i = '0' + rem1;
                }
                else if((rem / 10) == 1){
                	rem1 = rem % 10;
                	i  = 'A' + rem1;
                }
		printChar('0');
		printChar('x');
        	printChar(i);
        	return;
        }
        if((rem / 10) < 1){
                rem1 = rem % 10;
                i = '0' + rem1;
        }
        else if((rem / 10) == 1){
                rem1 = rem % 10;
                i  = 'A' + rem1;
        }
        printChar(i);
}

void printHex(unsigned long hArg){
	int rem=0, rem1=0;
	int i=0;
	rem = hArg % 16;
	hArg = hArg/16;
        if(hArg > 0) {
		printHex(hArg);
        }
	else if(hArg == 0){
		if((rem / 10) < 1){
			rem1 = rem % 10;
			i = '0' + rem1;
		}
        	else if((rem / 10) == 1){
			rem1 = rem % 10;
        		i  = 'A' + rem1;
        	}	
        	printChar(i);
		return;
	}
	if((rem / 10) < 1){
                rem1 = rem % 10;
                i = '0' + rem1;
        }
                else if((rem / 10) == 1){
                rem1 = rem % 10;
                i  = 'A' + rem1;
        }
        printChar(i);
}

void kprintf(const char *fmt, ...) {
    va_list vaList;
    int cArg, dArg=0;    
    char *sArg;
    int xArg, pArg;

    va_start(vaList, fmt);
    while( *fmt != 0 ) {
	    if(y > 22) {
		//y = 0;
		x = 0;
		doScroll();//Implement scrolling later 
	    }

	    if(x == 80) {
		x = 0;
		y++;
	    }

            if( *fmt == '\n' ) {
		vPointer = vPointer+(80-x)*2;
                y++;
                x = 0;
                fmt++;
		//*fmt = '\0';
            } 

            else if( *fmt == '\r' ) {
                x = 0;
                fmt++;
            }


	    else if( *fmt != '%' ) {
	    	printChar(*fmt);
	    	fmt++;
	    } else {
		fmt++;
		switch(*fmt) {
			case 'c': 
				cArg = va_arg(vaList, int);
				printChar(cArg);
				fmt++;
				break;

			case 's':
				sArg = va_arg(vaList, char *);
				while( *sArg != '\0' ) {
					if( *sArg == '\n' ) {
                				vPointer = vPointer+(80-x)*2;
                				y++;
                				x = 0;
                				sArg++;
                				//*fmt = '\0';
            				} else if( *sArg == '\r' ) {
                				x = 0;
                				sArg++;
            				}
					printChar(*sArg);
					sArg++;
				}
				fmt++;			
				break;	

			case 'd':
				dArg = va_arg(vaList, int);
				printInt((int)dArg);	
                                fmt++;
				break;
			case 'x':
				xArg = va_arg(vaList , unsigned long);
				printHex(xArg);
				fmt++;
				break;
			case 'p':
				pArg = va_arg(vaList , unsigned long);
                                printPointer(pArg);
                                fmt++;
                                break;
		}
	    }
    }
}

void printInt(int dArg){
	char a[20], b[20];
	int i = 0, j = 0;
	if(dArg == 0) {
                char d = '0';
                printChar(d);
                return;
        }
	int rem = 0;
	do {
		rem = dArg%10;
		a[i] = (char) (rem + 48);
		dArg = dArg/10;
		i++;
	} while(dArg > 0);

	while(i>0) {
		i--;
		b[j] = a[i];
		j++;
	}
	b[j] = '\0';
	j = 0;
	while( b[j] != '\0' ) {
	        printChar(b[j]);
                j++;
        }

	
}

/*
void printInt(int dArg){
	if(dArg == 0) {
		char d = '0';
		printChar(d);
		return;
	} else if(dArg < 0) {
		dArg *= -1;
		char c = '-';
		printChar(c);
	} 
	printInt(dArg/10);
        dArg = dArg %10;
        char c = '0'+dArg;
        printChar(c);
}
*/
/*void printint(int val)
{
        char str[100];
        str[0]='\0';
        int rem,i=0;
        while(val)
        {
                rem=val%10;
                str[i]=rem +'0';
                val=val/10;
                i++;
        }
//      *video--;
        i=i-1;
        while(i>=0)
        {
                *video++ =str[i];
                *video++ = 9;
                count=count+2;
                i--;

        }
}*/


void printTimeSmall(int tArg) {
         if(tArg == 0) {
                printChar('0');
                printChar('0');
                return;
        } else if(tArg>0 && tArg<10) {
                printChar('0');
		char d = '0'+tArg;
                printChar(d);
                return;
        }
}
void printTime(int tArg) {
	 if(tArg == 0) {
		printChar('0');
		printChar('0');
                return;
        } else if(tArg>0 && tArg<10) {
		char d = '0'+tArg;
		printChar(d);
		return;
	}
        printTime(tArg/10);
        tArg = tArg %10;
        char c = '0'+tArg;
        printChar(c);
}

void reverse(char *p)
{
    int length = sizeof(p), i=0;
    /*while(*p != '\0') {
	length++;
	p++;
    }*/
    
    char *q='\0';
    for(i=0; i<length; i++) {
	q[i] = p[length - 1 - i];
    }

    printChar('q');
    while( *q != '\0' ) {
        printChar(*q);
        q++;
    }
    
    printChar('q');
}

void kprintfWithPos(int xCord, int yCord, const char *fmt, ...) {
    va_list vaList;
    int cArg, dArg=0, tArg=0;
    char *sArg;
    int xArg, pArg;
    int xTemp = x;
    int yTemp = y;

    x = xCord;
    y = yCord;

    va_start(vaList, fmt);
    while( *fmt != 0 ) {
	    /*if(y > 22) {
                //y = 0;
                x = 0;
                doScroll();//Implement scrolling later
            } */

            if(x == 80) {
                x = 0;
                y++;
            }

            if( *fmt == '\n' ) {
                y++;
                x = 0;
                fmt++;
            }

            if( *fmt == '\r' ) {
                x = 0;
                fmt++;
            }


            if( *fmt != '%' ) {
                printChar(*fmt);
                fmt++;
            } else {
                fmt++;
                switch(*fmt) {
                        case 'c':
                                cArg = va_arg(vaList, int);
                                printChar(cArg);
                                fmt++;
                                break;

                        case 's':
                                sArg = va_arg(vaList, char *);
                                while( *sArg != '\0' ) {
				printChar(*sArg);
                                        sArg++;
                                }
                                fmt++;
                                break;

                        case 'd':
                                dArg = va_arg(vaList, int);
                                printInt((int)dArg);
                                fmt++;
                                break;
			case 't': 
				tArg = va_arg(vaList, int);
				if(tArg>0 && tArg<10)
				printTimeSmall((int)tArg);
				else
				printTime((int)tArg);
				fmt++;
				break;
                        case 'x':
                                xArg = va_arg(vaList , unsigned long);
                                printHex(xArg);
                                fmt++;
                                break;
                        case 'p':
                                pArg = va_arg(vaList , unsigned long);
                                printPointer(pArg);
                                fmt++;
                                break;
                }
            }
    }
    x = xTemp;
    y = yTemp;
}

                                                                                                
