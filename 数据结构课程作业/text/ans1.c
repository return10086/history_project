#include <stdio.h>
#include <string.h>
int num[10];

int main()
{
    memset(num, 0, sizeof(num));
    for (int i = 1; i <= 2020; i++)
    {
        int j = i;
        int p = 0;
        while (j != 0)
        {
            p = j % 10;
            j = j / 10;
            num[p]++;
        }
    }
    printf("%d", num[2]);
    return 0;
}