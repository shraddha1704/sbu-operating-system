#include <sys/printf.h>
#include <stdarg.h>
#define bufferSize 1000
#include <sys/defs.h>
#include <sys/string.h>
#include "sys/syscalls.h"

char read_buffer[bufferSize];
int buff_offset = 0;
int ret=0;

char * reverseString(char *dArgList);
void reverse(char *p);
void printInt(int num);
void printChar(char c);
void printPointer(unsigned long  pArg);
void printHex(unsigned long hArg);
void printTimeSmall(int tArg);
void printTime(int tArg);
void reverse(char *p);


void printChar(char c) {
    read_buffer[buff_offset] = c;
    buff_offset++;
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
void printf(const char *fmt, ...) {
    va_list vaList;
    int cArg, dArg=0;
    char *sArg;
    int xArg, pArg;

    va_start(vaList, fmt);
    while( *fmt != 0 ) {
            if( *fmt == '\n' ) {
                printChar('\n');
                fmt++;
                //*fmt = '\0';
            }
            else if( *fmt == '\r' ) {
                printChar('\r');
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
    read_buffer[buff_offset] = '\0';
    //write(1,&read_buffer,buff_offset);
	
    __asm__ ("int $0x80"
         : "=a" (ret)
         : "a" (4), "b" (1), "c" (read_buffer), "d" (sizeof(read_buffer)-1)
         : "memory"
        );


    for(int i=0;i<bufferSize;i++) {
        read_buffer[i] = '\0';
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

void reverse(char *p) {
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
