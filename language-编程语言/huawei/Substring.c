#include <stdio.h>

int substring(char* str1, char* str2)
{
    int idx1 = 0;
    int idx2 = 0;

    while (str1[idx1] != '\0')
    {
        if (str1[idx1] == str2[idx2])
        {
            idx2++;
        }
        else
        {
            idx1 -= idx2;
            idx2 = 0;
        }

        if (str2[idx2] == '\0')
        {
            return (idx1 - idx2);
        }
        idx1++;
    }
    return 0;
}

int substring2(char* str1, char* str2)
{
    int idx1 = 0;
    int idx2 = 0;

    while (str1[idx1] != '\0')
    {
        if (str1[idx1] == str2[idx2])
        {
            idx2++;
        }
        else
        {
            idx1 -= idx2;
            idx2 = 0;
        }

        if (str2[idx2] == '\0')
        {
            return (idx1 - idx2 + 1);
        }
        idx1++;
    }
    return -1;
}

int main(void)
{
    int off = 0;
    char  str1[1000];
    char  str2[1000];

    gets(str1);
    gets(str2);

    if ((off = substring2(str1, str2)) >= 0)
    {
        printf("OK [%s]", str1 + off);
    }
    else
    {
        printf("NG");
    }

    return 0;
}