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
	const char *target = dest; 
	while(*src!='\0'){
		*dest = *src;
		src++;
		dest++;
	}
	*dest = '\0';

	return target;
}


int strlen(const char *s) {
        int len;
        for (len = 0; *s != '\0'; s++) {
                len++;
        }
        return len;
}

int strcmp(const char *str1, const char *str2) {
        while (*str1 != '\0' || *str2 != '\0') {
                if (*str1 != *str2) {
			return -1;
		}
                str1++;
		str2++;
        }
        return 0;
}

int strncmp(const char *str1, const char *str2, size_t n) {
	int diff = 0;
        while (n > 0 && *str1 && *str1 == *str2) {
		n--;
		str1++;
		str2++;
	}
        if (n == 0) {
		return diff;
	}
        else {
		diff = (int) ((unsigned char) *str1 - (unsigned char) *str2);
		return diff;
	}
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

void strcat(char *newString, const char *originalString) {
	while (*newString!= '\0') {
        	newString++;
	}
	do {
	        *newString++ = *originalString++;
    	}
    	while (*originalString != '\0') ;
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

