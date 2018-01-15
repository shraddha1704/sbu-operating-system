#ifndef __KPRINTF_H
#define __KPRINTF_H

void kprintf(const char *fmt, ...);
void kprintfWithPos(int xCord, int yCord, const char *fmt, ...);
void clearScreen();
void clearChar();
/*void printTime(int count);
void tprintf(const char *fmt, ...);

void kprintfWithPos(int xCord,int yCord, const char* fmt, ...);
void sprintf(const char *fmt, ...);*/
#endif
