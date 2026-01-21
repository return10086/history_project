#include <stdio.h>
#include <string.h>
// 求最大公约数
int getD(int a, int b);

int main()
{
    return 0;
}

// 求最大公约数
int getD(int a, int b)
{
    if (a % b == 0)
        return b;
    int temp = 0;
    while (1)
    {
        temp = a % b;
        a = b;
        b = temp;
    }
}
