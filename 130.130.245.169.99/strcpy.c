#include<custom.h>

char* strcpy(char *str1, char *str2)
{

        while(*str2)
        {
                *str1++ = *str2++;

        }

        return str1;

}

