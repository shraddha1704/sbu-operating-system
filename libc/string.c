#include <sys/string.h>
#include <sys/defs.h>

int strtok(char* input, char **tokens,char separator){
        int token_count = 0;
        while(*input==' '){
            input++;
        }
        while(*input!='\0'){

                //Token saves the address of the first letter of each word.
                while (*input ==' '){
                    input++;
                }
                *tokens++ = input;
                token_count++;
                while(*input!=separator&& *input!='\0')
                {

                        input++;
                }

                //Once End of String is reached

                if (*input=='\0')
                        continue;

                *input = '\0';
                input++;
        }

        *tokens = '\0';
        return token_count;
}

const char* strcpy(char *dest, const char *src) {
	const char *saved = dest; 

	while(*src){
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';

	return saved;
}


int strlen(const char *s) {
        int n;
        for (n = 0; *s != '\0'; s++) {
                n++;
        }
        return n;
}

int strcmp(const char *p, const char *q) {
        while (*p || *q) {
                if (*p != *q)
                        return -1;
                p++, q++;
        }
        return 0;
}

int strncmp(const char *p, const char *q, size_t n) {
        while (n > 0 && *p && *p == *q)
                n--, p++, q++;
        if (n == 0)
                return 0;
        else
                return (int) ((unsigned char) *p - (unsigned char) *q);
}

uint64_t charToInt(char *s) {
        uint64_t i;
        i = 0;
        while(*s >= '0' && *s <= '9') {
                i = i * 10 + (*s - '0');
                s++;
        }
        return i;
}

void strcat(char *newString, const char *orig) {
	while (*newString!= '\0')
        	newString++;
	do {
	        *newString++ = *orig++;
    	}
    	while (*orig != '\0') ;
	*newString = '\0';
}

void memcpy(void *destination, void *source, uint64_t length) {
	char *d = (char *) destination;
	char *s = (char *) source;
	for(int i = 0; i<length; i++) {
		*d++ = *s++;
	}
}

void* memset(void* target, int val, int length) {
        unsigned char *p = target;
        while(length>0) {
                *p = val;
                p++;
                length--;
	}
        return target;
}

void memset1(void* target, int val, int length) {
        unsigned char *p = target;
        while(length>0) {
                *p = val;
                p++;
                length--;
        }
}

