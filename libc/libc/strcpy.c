#include<custom.h>

char* strcpy(char *str1, char *str2)
{

        while(*str2)
        {
                *str1++ = *str2++;

        }

        return str1;

}

int strlen(const char *s) {
	int n;

	for (n = 0; *s != '\0'; s++)
		n++;
	return n;
}
