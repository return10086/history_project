#include <stdio.h>
//#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#define stack_init_size 100
#define stackincrement 10
//优先级等级
typedef enum priority
{
    A = 1,
    B,
    C,
    D,
    E,
    F,
    Z
} priority;

//位置信息
typedef struct point
{
    int x; // x坐标
    int y; // y坐标
    priority p[4];
} point;

//定义顺序栈
typedef struct step
{
    point *base;
    point *top;
    int stacksize;
} step;
int flag = 0;
#define map_x 20
#define map_y 20
char map[map_y][map_x];
//初始化栈
step *initlisttack(void);
//取出栈顶元素
void GetTop(step *s, point *e);
//入栈 e为新的栈顶元素
void push(step *s, int mx, int my, char *m);
//出栈，用e返回其值
point pop(step *s);
//销毁栈
void DestroyStack(step *s);
//打印栈
void print(step *s);
//寻找起点终点
void scan(char *m, point *e, point *s);
//比对两个栈元素是否相同
char compare(step *M, point *e);
//判断优先级
priority judge_priority(char *m, int x, int y);
//第二个思路：优先级路线
int go2(step *this_step, point *e, point *x, char *m);
//查询上下左右四个点优先级
void search(int x, int y, point *e, char *m);
//找优先级最高的点
char max_priority(int x, int y, char *m, point *e);
//写电流函数
void watch(point *e, point *s, char *m);
//读地图
int readFile(const char *file_name);
//打印地图
void print_map(char *m, int x, int y);
int main()
{

    readFile("test1.txt"); //读取文件
    step *m1 = initlisttack();
    point e = {0, 0, {A, A, A, A}};
    point s = {0, 0, {A, A, A, A}};
    scan((char *)map, &e, &s); //找到了起点终点,并且把每个点优先级设为最高
    printf("起点是：（%d,%d)\r\n终点是：(%d,%d)\r\n", e.x, e.y, s.x, s.y);
    int step = go2(m1, &e, &s, (char *)map);
    printf("需要%d步走到终点\r\n", step);
    print(m1);
    print_map((char *)map, 20, 20);
    system("pause");
    return 0;
}

//初始化栈
step *initlisttack(void)
{
    step *s = (step *)malloc(stack_init_size * sizeof(step));
    if (!s)
        ;
    s->base = (point *)malloc(stack_init_size * sizeof(point));
    if (!s->base)
        ;
    s->top = s->base; //初始位置，栈顶等同于栈底
    s->stacksize = stack_init_size;
    return s;
}
//取出栈顶元素
void GetTop(step *s, point *e)
{
    if (s->top != s->base)
        e = s->top - 1;
}
//入栈 e为新的栈顶元素
void push(step *s, int mx, int my, char *m)
{
    if (s->top - s->base >= s->stacksize) //如果栈满
    {
        s->base = (point *)realloc(s->base, (s->stacksize + stackincrement) * sizeof(point));
        if (!s->base)
            exit(_OVERFLOW);
        s->top = s->base + s->stacksize;
        s->stacksize += stackincrement;
    }
    point *e = (point *)malloc(sizeof(point));
    if (e != NULL)
    {
        e->x = mx;
        e->y = my;
    }
    search(mx, my, e, m);
    *s->top++ = *e;
}
//出栈，用e返回其值
point pop(step *s)
{
    point h = *s->top;
    s->top -= 1;
    return h;
}
//销毁栈
void DestroyStack(step *s)
{
    free(s);
}
//打印栈
void print(step *s)
{
    point *temp;
    temp = s->base;
    while (temp != s->top)
    {
        printf("(%d,%d)\r\n", temp->x, temp->y);
        temp = temp + 1;
    }
}
//扫描结点，找到起点终点,并将每个可走点置为最高级
void scan(char *m, point *e, point *s)
{
    int num = map_x * map_y;
    int i, j;
    char *n = m;
    for (j = 0; j < map_y; j++) // y循环
    {
        for (i = 0; i < map_x; i++) // x循环
        {
            if (*n == 'E')
            {
                if (e != NULL)
                {
                    e->x = i;
                    e->y = j;
                }
                search(i, j, e, m);
            }
            if (*n == 'S')
            {
                s->x = i;
                s->y = j;
                search(i, j, s, m);
            }
            if (*n == ' ')
            {
                *n = 'A'; //最初都置为最高优先级
            }
            n = n + 1;
        }
    }
}
//比对两个栈元素是否相同(返回栈顶之下几个元素和要对比的栈相同）
char compare(step *m, point *e)
{
    point *t; //遍历指针
    t = m->top;
    char num = 0;
    while (t->x != e->x || t->y != e->y) //只要t不和要对比的坐标相同就一直运行
    {
        if (t == m->base) //到底了
        {
            num = 0;
            break;
        }
        t = t - 1; //指向下一个位置
        num += 1;
    }
    return num - 1;
}
//判断优先级(输入一个点的坐标，返回其极性）
priority judge_priority(char *m, int x, int y)
{
    if (x >= map_x || y >= map_y || x < 0 || y < 0)
    {
        return E; //墙为最低优先级
    }
    char *n = m;
    n = n + (map_x * y + x); //获取点信息
    switch (*n)
    {
    case 'A':
        return A;
        break;
    case 'B':
        return B;
        break;
    case 'C':
        return C;
        break;
    case 'D':
        return D;
        break;
    case ' ':
        return A;
        break;
    case 'S':
        return A;
        break;
    case '#':
        return Z;
        break;
    default:
        return E;
        break;
    }
}
//第二个思路：优先级路线
/*
本思路主要思想：
初始状态所有可以走的点均为最高优先级
每走过一个点将此点优先级降级
选择下一个走的方向顺序为（上右下左）

本思路由两种算法支持：
1、遇到已经走过的点，退栈到第一次走这个点的时候，并把上一次走过的路径从中间打隔断，断为两个死胡同
2、如果走到死胡同，就把一边退栈一边把死胡同降优先级
*/
int go2(step *this_step, point *e, point *s, char *m)
{
    //调试时程序经常在while循环跑飞，stop标志位做看门狗，程序跑飞马上跳出程序
    int stop = 0;
    //储存地图
    char *n = m;
    //遍历节点
    point *g = e;
    while (1)
    {
        //如果找到了目标点就退出
        if (e->x == s->x && e->y == s->y)
        {
            //返回栈大小
            return this_step->top - this_step->base;
        }
        if (stop >= 500)
        {
            return -1;
            break; //如果循环次数大于500了，马上跳出循环
        }
        stop++;
        //第一步：重新查询四个点优先级
        search(g->x, g->y, g, m);
        //第二步：判断栈的前面有没有和这次结点相同的点，如果有，把前面的结点都退栈，并把路封死
        char o = compare(this_step, g);
        char t = o;
        if (o > 0) //如果前面有这个点的话
        {
            point *tt = g;
            while (o >= 0) //如果前面走过这个点了
            {
                if (o == t / 2 + 1) //如果退到中间一点了,就把点写死
                {
                    n = n + (map_x * tt->y + tt->x);
                    *n = 'Z';
                    n = m;
                }
                else //如果不是中间一点，把点都升优先级，变成走之前的样子
                {
                    n = n + (map_x * tt->y + tt->x);
                    *n = *n - 1;
                    n = m;
                }
                o--;
                tt = this_step->top - 1; // g始终指向顶部元素
                pop(this_step);          //退栈
            }
            g->x = tt->x;
            g->y = tt->y;
            search(g->x, g->y, g, m);
        }
        //第三步：把g写入栈
        push(this_step, g->x, g->y, m);
        // print_map((char *)map, 20, 20);
        // printf("\n\n\n\n");
        //第四步：把当前点优先级降级
        n = n + (map_x * g->y + g->x); //获取点信息
        if (*n != 'E' && *n != 'S')
        {
            *n = *n + 1;
        }
        n = m;
        //第五步：找优先级最高的点
        char tt = max_priority(g->x, g->y, m, g);
        int hx = g->x;
        int hy = g->y;
        switch (tt) //找到了新的g
        {
        case 0:
            g->x = g->x;
            g->y = g->y - 1;
            if (g->x == (this_step->top - 2)->x && g->y == (this_step->top - 2)->y)
            {
                pop(this_step);
                pop(this_step);
                flag = 1;
            }
            else
            {
                if (flag)
                {
                    n = n + (map_x * hy + hx); //获取点信息//升优先级
                    *n = *n - 1;
                    n = m;
                    flag = 0;
                }
            }
            break;
        case 1:
            g->x = g->x + 1;
            g->y = g->y;
            if (g->x == (this_step->top - 2)->x && g->y == (this_step->top - 2)->y)
            {
                pop(this_step);
                pop(this_step);
                flag = 1;
            }
            else
            {
                if (flag)
                {
                    n = n + (map_x * hy + hx); //获取点信息//升优先级
                    *n = *n - 1;
                    n = m;
                    flag = 0;
                }
            }
            break;
        case 2:
            g->x = g->x;
            g->y = g->y + 1;
            if (g->x == (this_step->top - 2)->x && g->y == (this_step->top - 2)->y)
            {
                pop(this_step);
                pop(this_step);
                flag = 1;
            }
            else
            {
                if (flag)
                {
                    n = n + (map_x * hy + hx); //获取点信息//升优先级
                    *n = *n - 1;
                    n = m;
                    flag = 0;
                }
            }
            break;
        case 3:
            g->x = g->x - 1;
            g->y = g->y;
            if (g->x == (this_step->top - 2)->x && g->y == (this_step->top - 2)->y)
            {
                pop(this_step);
                pop(this_step);
                flag = 1;
            }
            else
            {
                if (flag)
                {
                    n = n + (map_x * hy + hx); //获取点信息//升优先级
                    *n = *n - 1;
                    n = m;
                    flag = 0;
                }
            }
            break;
        default:
            break;
        }
        //如果当前新g与栈顶部元素相同，说明走到了死胡同，退栈
    }
    return this_step->top - this_step->base;
}
//查询上下左右四个点优先级
void search(int x, int y, point *e, char *m)
{
    for (int t = 0; t < 4; t++) //查询上右下左四个点
    {
        if (e != NULL)
            switch (t)
            {
            case 0: //上
                e->p[0] = judge_priority(m, x, y - 1);
                break;
            case 1: //右
                e->p[1] = judge_priority(m, x + 1, y);
                break;
            case 2: //下
                e->p[2] = judge_priority(m, x, y + 1);
                break;
            case 3: //左
                e->p[3] = judge_priority(m, x - 1, y);
                break;
            }
    }
}
//四个点找优先级最高,(把s置为最高优先级）
char max_priority(int x, int y, char *m, point *e)
{
    priority t = e->p[0];
    (t < e->p[1]) ? (t = t) : (t = e->p[1]);
    (t < e->p[2]) ? (t = t) : (t = e->p[2]);
    (t < e->p[3]) ? (t = t) : (t = e->p[3]);
    //此时t为优先级最高的方向
    if (t == e->p[0])
        return 0;
    else if (t == e->p[1])
        return 1;
    else if (t == e->p[2])
        return 2;
    else
        return 3;
}
//读地图
int readFile(const char *file_name)
{
    FILE *fp; /*文件指针*/
    char i = 0;
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        perror("fail to read");
        exit(1);
    }
    while (fgets(map[i++], 200, fp) != NULL)
        ;
}

//打印地图
void print_map(char *m, int x, int y)
{
    char *n = m;
    for (char j = 0; j < y; j++)
    {
        for (char i = 0; i < x; i++)
        {
            printf(" %c ", *n);
            n++;
        }
        printf("\n");
    }
}