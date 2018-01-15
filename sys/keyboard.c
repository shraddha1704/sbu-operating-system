#include "sys/defs.h"
#include "sys/outbandinb.h"
#include "sys/irq.h"
#include "sys/kprintf.h"

int timer_tic = 0;
int sec = 1;
int shiftFlag = 0, controlFlag = 0;

volatile int scanFlag = 0;
char scanBuffer[1024];
int scanLength = 0;

void memcpy2(void *dest, void *src, uint64_t len)
{
    char *pd = (char *)dest;
    char *ps = (char *)src;
    int i=0;
    while(i<len)
    {
        *pd++=*ps++;
        i++;
    }
    *pd='\0';
}

void memset2(void * ptr,int val,int size) {
        for(int i=0;i<size;i++)
                ((char*)(ptr))[i]=val;
}


int readFromStdin(void * address) {

	__asm__ volatile("sti");

	memset2(scanBuffer, '\0', 1024);
	scanFlag = 1;
        void * dest = (void *) address;
        char * src = (char *) scanBuffer;
	//kprintf("-----------");
        int scanBufferLength = 0;
        while(scanFlag != 0) {
                scanBufferLength++;
         //       *dest++ = *src;
         //       *src = '\0';
         //       *src++;
		//kprintf("not equal to 0\n");
        }

	memcpy2(dest, (void *)src, scanLength);
	memset2(scanBuffer, '\0', scanLength);

	scanFlag = 0;
	scanLength = 0;
	//kprintf("equal to 0 \n");
	__asm__ volatile("cli");
        return scanBufferLength;
}

void testBuffer() {
	kprintf("in test buffer \n");
	for(int i = 0; i<scanLength; i++) {
		kprintf("%c",scanBuffer[i]);
	}
	scanFlag = 0;
	scanLength = 0;
}

unsigned char usKeyboard[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    '~',			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   '^',		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   '^',				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


void keyboard_handler() {

	unsigned char scanCode = inb(0x60);
//      kprintf("------------Inside keyboardCb scan --------", scanCode);

        if(scanCode & 0x80) {
                //do nothing
                if(scanCode == 0xAA) {
                        shiftFlag = 0;
                }
                if(scanCode == 0x9D) {
                        controlFlag = 0;
                }
                //controlFlag = 0;
        } else {
                char key = usKeyboard[scanCode];

                if(key == '~') {
                        controlFlag = 1;
                } else if(key == '^') {
                        shiftFlag = 1;
                } else if (key == '\n') {
			if(scanFlag == 1) {
				scanFlag = 0;
				scanBuffer[scanLength] = '\0';
				kprintf("\n");
				//__asm__ volatile("cli");
                        	//testBuffer();
			}
			//kprintf("\n");
                        //kprintf("%c%c", '^','M');
                } else if (key == '\t') {
			kprintf(" ");
                        //kprintf("%c%c", '^','I');
                }  else {
                        if(shiftFlag == 1) {
                                //shiftFlag = 0;
                                switch(key) {
                                        case '1':
						if(scanFlag == 1) {
							scanBuffer[scanLength++] = '!';
							kprintf("%c", '!');
						}
                                                //kprintf("%c", '!');
                                                break;
                                        case '2':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '@';
							kprintf("%c", '@');
                                                }
                                                //kprintf("%c", '@');
                                                break;
                                        case '3':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '#';
							kprintf("%c", '#');
                                                }
                                                //kprintf("%c", '#');
                                                break;
                                        case '4':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '$';
							kprintf("%c", '$');
                                                }
                                                //kprintf("%c", '$');
                                                break;
					case '5':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '%';
							kprintf("%c", '%');
                                                }
                                                //kprintf("%c", '%');
                                                break;
                                        case '6':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '^';
							kprintf("%c", '^');
                                                }
                                                //kprintf("%c", '^');
                                                break;
                                        case '7':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '&';
							kprintf("%c", '&');
                                                }
                                                //kprintf("%c", '&');
                                                break;
                                        case '8':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '*';
							kprintf("%c", '*');
                                                }
                                                //kprintf("%c", '*');
                                                break;
                                        case '9':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '(';
							kprintf("%c", '(');
                                                }
                                                //kprintf("%c", '(');
                                                break;
                                        case '0':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = ')';
							kprintf("%c", ')');
                                                }
                                                //kprintf("%c", ')');
                                                break;
                                        case '-':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '_';
							kprintf("%c", '_');
                                                }
                                                //kprintf("%c", '_');
                                                break;
                                        case '=':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '+';
							kprintf("%c", '+');
                                                }
                                                //kprintf("%c", '+');
                                                break;
                                        case '`':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '~';
							kprintf("%c", '~');
                                                }
                                                //kprintf("%c", '~');
                                                break;
                                        case '[':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '{';
							kprintf("%c", '{');
                                                }
                                                //kprintf("%c", '{');
                                                break;
                                        case ']':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '}';
							kprintf("%c", '}');
                                                }
                                                //kprintf("%c", '}');
                                                break;
                                        case ';':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = ':';
							kprintf("%c", ':');
                                                }
                                                //kprintf("%c", ':');
                                                break;
                                        case '\'':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '\"';
							kprintf("%c", '\"');
                                                }
                                                //kprintf("%c", '\"');
                                                break;
                                        case ',':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '<';
							kprintf("%c", '<');
                                                }
                                                //kprintf("%c", '<');
					case '.':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '>';
							kprintf("%c", '>');
                                                }
                                                //kprintf("%c", '>');
                                                break;
                                        case '/':
						if(scanFlag == 1) { 
                                                        scanBuffer[scanLength++] = '?';
							kprintf("%c", '?');
                                                }
                                                //kprintf("%c", '?');
                                                break;
                                        default:
                                                //kprintf("%c", usKeyboard[scanCode] - 32);
						if(scanFlag == 1) {
							kprintf("%c", usKeyboard[scanCode] - 32);
							scanBuffer[scanLength++] = usKeyboard[scanCode] - 32;
						}
                                }
                        } else if(controlFlag == 1 && scanFlag == 1) {
                                //controlFlag = 0;
                                kprintf("%c", '^');
                                kprintf("%c", usKeyboard[scanCode] - 32);
                        } else {
                                if(scanCode == 14) {
					if(scanFlag == 1) {
						if(scanLength>=0) {
							scanBuffer[--scanLength] = ' ';
							clearChar();
						}
					}
                                        //kprintf("%c", '^');
                                        //kprintf("%c", 'H');
                                } else if(scanCode == 83 && scanFlag==1) {
                                        kprintf("%c", '^');
                                        kprintf("%c", '?');
                                } else if(scanCode == 1 && scanFlag == 1) {
                                        kprintf("%c", '^');
                                        kprintf("%c", '[');
                                } else {
                                        //kprintf("%c", key);
					if(scanFlag == 1) {
						kprintf("%c", key);
						scanBuffer[scanLength++] = key;
					}
                                }
                        }
                }
        }
}




