#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MAXINIT 32767

#define block_num 10
#define map_x 4
#define map_y 5
int tar_x;
int tar_y;

typedef enum //板块下一步方向
{
    left = -2,
    up,
    stop, //没有方向可走
    down,
    right
} dir;

typedef struct //板块结构体
{
    int code;
    int x;
    int y;
    int w; // width
    int h;
} Block;

typedef struct solution
{
    int map[map_y][map_x];   //局面
    struct solution *father; //父结点
    struct solution *pre;    //上一个图
    struct solution *next;   //下一个图
} Solution;                  //别名

/* 循环队列的顺序存储结构 */
typedef struct
{
    struct solution *front; /* 首方案 */
    struct solution *rear;  /* 末方案，若队列不空，指向队列尾元素的下一个位置 */
} SqQueue;

/***********************队列操作**********************/

/* 初始化一个空队列Q */
SqQueue *InitQueue(void);
/* 若队列Q为空队列,则返回1,否则返回0 */
int QueueEmpty(SqQueue Q);
/* 若队列不空,则用e返回Q的队头元素,并返回1,否则返回0 */
Solution *GetHead(SqQueue Q);
/* 若队列未满，则插入元素e为Q新的队尾元素 */
void EnQueue(SqQueue *Q, int e[5][4], Solution *now);
/* 若队列不空，则删除Q中队头元素，用e返回其值 */
void DeQueue(SqQueue *Q);

/*判断板块B能不能在地图M中向D方向移动*/
int move_enable(int M[map_y][map_x], int B, dir D);
/*板块B能不能在地图M中向D方向移动*/
void move(int M[map_y][map_x], int B, dir D);
/*打印移动步骤*/
void print(Solution *h);
/*该新局面是否已经在前面某方块移动后产生过*/
int watch_back(SqQueue *Q, Solution *now);
/*读地图*/
void read_txt(void);
/*写到txt里*/
void print2(Solution *h);
SqQueue *my_Queue;      //全局队列
Block block[block_num]; //所有板块，第一个是曹操
Solution first_state;   //初始状态
int main()
{
    my_Queue = InitQueue(); //初始化队列
    read_txt();

    EnQueue(my_Queue, first_state.map, NULL); //把初始状态入队
    Solution *now = my_Queue->rear;
    while (now != NULL)
    {
        //从now中获取板块位置
        int num[block_num];
        for (int i = 0; i < block_num; i++)
            num[i] = 0;
        for (int i = map_y - 1; i >= 0; i--)
        {
            for (int j = 0; j < map_x; j++)
            {
                if (now->map[i][j] != MAXINIT)
                    if (num[now->map[i][j]] != 1) //如果这是没有找到过的结点
                    {
                        block[now->map[i][j]].x = j;
                        block[now->map[i][j]].y = i;
                        num[now->map[i][j]] = 1; //标记已经找过这个点了
                    }
            }
        }
        if (now->map[0][0] == 0 && now->map[0][1] == 0 && now->map[0][2] == 2 && now->map[0][3] == MAXINIT && now->map[1][0] == 0 && now->map[1][1] == 0 && now->map[1][2] == 1 && now->map[1][3] == 4 && now->map[2][0] == MAXINIT && now->map[2][1] == MAXINIT && now->map[2][2] == 1 && now->map[2][3] == 3 && now->map[3][0] == MAXINIT && now->map[3][1] == MAXINIT && now->map[3][2] == MAXINIT && now->map[3][3] == 3)
        {
            int c = 3;
        }
        for (int i = 0; i < block_num; i++)
        {
            if (move_enable(now->map, i, left)) //板块i能否向左移
            {
                move(now->map, i, left); //移动该板块,获取一个新局面
                //对新局面进行判断，曹操是否已经走到出口
                if (block[0].x == tar_x && block[0].y == tar_y) //走到出口
                {

                    EnQueue(my_Queue, now->map, now);
                    move(now->map, i, -left);
                    print2(my_Queue->rear); //打印移动步骤
                    system("pause");        //退出程序
                }
                //该新局面是否已经在前面某方块移动后产生过
                if (watch_back(my_Queue, now)) //如果产生过
                    ;                          //直接跳
                else                           //未产生过
                {
                    EnQueue(my_Queue, now->map, now); //保存起来
                }
                move(now->map, i, -left); //将该板块移动回原位，方便进行下一个移动方向判断
            }
            if (move_enable(now->map, i, up)) //板块i能否向上移
            {
                move(now->map, i, up); //移动该板块,获取一个新局面
                //对新局面进行判断，曹操是否已经走到出口
                if (block[0].x == tar_x && block[0].y == tar_y) //走到出口
                {
                    EnQueue(my_Queue, now->map, now);
                    move(now->map, i, -up);
                    print2(my_Queue->rear); //打印移动步骤
                    system("pause");        //退出程序
                }
                //该新局面是否已经在前面某方块移动后产生过
                if (watch_back(my_Queue, now)) //如果产生过
                    ;                          //直接跳
                else                           //未产生过
                {
                    EnQueue(my_Queue, now->map, now); //保存起来
                }
                move(now->map, i, -up); //将该板块移动回原位，方便进行下一个移动方向判断
            }
            if (move_enable(now->map, i, right)) //板块i能否向右移
            {
                move(now->map, i, right); //移动该板块,获取一个新局面
                //对新局面进行判断，曹操是否已经走到出口
                if (block[0].x == tar_x && block[0].y == tar_y) //走到出口
                {
                    EnQueue(my_Queue, now->map, now);
                    move(now->map, i, -right);
                    print2(my_Queue->rear); //打印移动步骤
                    system("pause");        //退出程序
                }
                //该新局面是否已经在前面某方块移动后产生过
                if (watch_back(my_Queue, now)) //如果产生过
                    ;                          //直接跳
                else                           //未产生过
                {
                    EnQueue(my_Queue, now->map, now); //保存起来
                }
                move(now->map, i, -right); //将该板块移动回原位，方便进行下一个移动方向判断
            }
            if (move_enable(now->map, i, down)) //板块i能否向下移
            {
                move(now->map, i, down); //移动该板块,获取一个新局面
                //对新局面进行判断，曹操是否已经走到出口
                if (block[0].x == tar_x && block[0].y == tar_y) //如果找到目标状态了
                {
                    EnQueue(my_Queue, now->map, now);
                    move(now->map, i, -down);
                    print2(my_Queue->rear); //打印移动步骤
                    system("pause");        //退出程序
                }
                //该新局面是否已经在前面某方块移动后产生过
                if (watch_back(my_Queue, now)) //如果产生过
                    ;
                else //未产生过
                {
                    EnQueue(my_Queue, now->map, now);
                }
                move(now->map, i, -down); //将该板块移动回原位，方便进行下一个移动方向判断
            }
        }
        now = now->next;
    }
    printf("此图无解");
    system("pause");
}

/* 初始化一个空队列Q */
SqQueue *InitQueue(void)
{
    SqQueue *Q;
    Q = (SqQueue *)malloc(sizeof(SqQueue));
    Q->rear = Q->front = (Solution *)malloc(sizeof(Solution));

    return Q;
}

/* 若队列Q为空队列,则返回1,否则返回0 */
int QueueEmpty(SqQueue Q)
{
    if (Q.front == Q.rear) /* 队列空的标志 */
        return 1;
    else
        return 0;
}

/* 若队列不空,则用e返回Q的队头元素,并返回1,否则返回0 */
Solution *GetHead(SqQueue Q)
{
    Solution *e;
    if (Q.front == Q.rear) /* 队列空 */
        return NULL;
    e = Q.front - 1;
    return e;
}

/* 若队列未满，则插入元素e为Q新的队尾元素 */
void EnQueue(SqQueue *Q, int e[5][4], Solution *now)
{
    Solution *s = (Solution *)malloc(sizeof(Solution));
    for (int i = 0; i < map_y; i++)
    {
        for (int j = 0; j < map_x; j++)
            s->map[i][j] = e[i][j];
    }
    s->next = NULL;
    Q->rear->next = s; //把拥有元素e新结点s赋值给原队尾结点的后继
    s->pre = Q->rear;
    Q->rear = s; //把当前的s设置为新的队尾结点
    s->father = now;
}

/* 若队列不空，则删除Q中队头元素 */
void DeQueue(SqQueue *Q)
{
    Solution *p;
    if (Q->front == Q->rear)
    {
        return;
    }
    p = Q->front->next;       //将欲删除的队头结点暂存给p
    Q->front->next = p->next; //将原队头结点的后继赋值给头结点后继
    //若删除的队头是队尾，则删除后将rear指向头结点
    if (Q->rear == p)
    {
        Q->rear = Q->front;
    }
    free(p);
}

/*判断板块B能不能在地图M中向D方向移动*/
int move_enable(int M[map_y][map_x], int B, dir D)
{
    int move_enable_flag = 1;
    switch (D)
    {
    case left:
        if (block[B].x == 0) //板块在最左面
            return 0;        //不能左移
        for (int i = block[B].y; i > block[B].y - block[B].h; i--)
        {
            if (M[i][block[B].x - 1] != MAXINIT)
                move_enable_flag = 0;
        }
        break;
    case up:
        if (block[B].y == map_y - 1) //板块在最上面
            return 0;                //不能上移
        for (int i = block[B].x; i < block[B].x + block[B].w; i++)
        {
            if (M[block[B].y + 1][i] != MAXINIT)
                move_enable_flag = 0;
        }
        break;
    case right:
        if (block[B].x + block[B].w == map_x) //板块在最右面
            return 0;                         //不能右移
        for (int i = block[B].y; i > block[B].y - block[B].h; i--)
        {
            if (M[i][block[B].x + block[B].w] != MAXINIT)
                move_enable_flag = 0;
        }
        break;
    case down:
        if (block[B].y - block[B].h == -1) //板块在最下面
            return 0;                      //不能下移
        for (int i = block[B].x; i < (block[B].x + block[B].w); i++)
        {
            if (M[block[B].y - block[B].h][i] != MAXINIT)
                move_enable_flag = 0;
        }
        break;
    default:
        break;
    }
    return move_enable_flag;
}

/*移动板块*/
void move(int M[map_y][map_x], int B, dir D)
{
    switch (D)
    {
    case left:
        for (int j = block[B].y; j > block[B].y - block[B].h; j--)
        {
            M[j][block[B].x + block[B].w - 1] = MAXINIT;
        }
        for (int i = block[B].y; i > block[B].y - block[B].h; i--)
        {
            M[i][block[B].x - 1] = block[B].code;
        }
        block[B].x -= 1;
        break;
    case up:
        for (int j = block[B].x; j < block[B].x + block[B].w; j++)
        {
            M[block[B].y - block[B].h + 1][j] = MAXINIT;
        }
        for (int i = block[B].x; i < block[B].x + block[B].w; i++)
        {
            M[block[B].y + 1][i] = block[B].code;
        }
        block[B].y += 1;
        break;
    case right:
        for (int j = block[B].y; j > block[B].y - block[B].h; j--)
        {
            M[j][block[B].x] = MAXINIT;
        }
        for (int i = block[B].y; i > block[B].y - block[B].h; i--)
        {
            M[i][block[B].x + block[B].w] = block[B].code;
        }
        block[B].x += 1;
        break;
    case down:
        for (int j = block[B].x; j < (block[B].x + block[B].w); j++)
        {
            M[block[B].y][j] = MAXINIT;
        }
        for (int i = block[B].x; i < (block[B].x + block[B].w); i++)
        {
            M[block[B].y - block[B].h][i] = block[B].code;
        }
        block[B].y -= 1;
    default:
        break;
    }
}

/*该新局面是否已经在前面某方块移动后产生过*/
int watch_back(SqQueue *Q, Solution *now)
{
    int flag = 1;
    int num = 0;
    if (!QueueEmpty(*Q)) //如果队列不为空
    {
        Solution *p = Q->front;
        while (p != Q->rear)
        { //取得状态

            for (int i = 0; i < map_y; i++)
            {
                for (int j = 0; j < map_x; j++)
                {
                    if (p->map[i][j] != now->map[i][j]) //如果有一个元素不同
                    {
                        flag = 0;
                        goto jump;
                    }
                }
            }
        jump:
            if (flag) //如果地图完全相同
            {
                if (num)
                    return 1;
                else
                {
                    num = 1;
                    flag = 1;
                }
            }
            else
                flag = 1;
            p = p->next;
        }
        return 0;
    }
    else //如果队列为空
        return 0;
}

/*打印移动步骤*/
void print(Solution *h)
{
    Solution *p = h;
    while (h != NULL)
    {
        for (int i = map_y - 1; i >= 0; i--)
        {
            for (int j = 0; j < map_x; j++)
            {
                if (h->map[i][j] != MAXINIT)
                    printf(" %d", h->map[i][j]);
                else
                    printf(" +");
            }
            printf("\r\n");
        }
        h = h->father;
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
    }
}

/*读地图*/
void read_txt(void)
{
    //初始化板块
    for (int i = 0; i < block_num; i++)
        block[i].code = i;

    //做标记
    int num[block_num];
    for (int i = 0; i < block_num; i++)
        num[i] = 0;

    //读文件
    FILE *fp;
    char ch;
    fp = fopen("map.txt", "r");
    if (fp == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }
    ch = fgetc(fp);
    for (int i = map_y - 1; i >= 0; i--)
    {
        for (int j = 0; j < map_x; j++)
        {
            if (ch == '+')
            {
                first_state.map[i][j] = MAXINIT;
            }
            else if (ch == '\n')
            {
                j--;
            }
            else
            {
                first_state.map[i][j] = ch - 48;
            }
            ch = fgetc(fp);
            if (ch == -1)
                goto jump2;
        }
    }
jump2:
    //初始化
    for (int i = map_y - 1; i >= 0; i--)
    {
        for (int j = 0; j < map_x; j++)
        {
            if (first_state.map[i][j] != MAXINIT)
                if (num[first_state.map[i][j]] == 0) //如果没被写过
                {
                    block[first_state.map[i][j]].x = j;
                    block[first_state.map[i][j]].y = i;
                    block[first_state.map[i][j]].h = 0;
                    block[first_state.map[i][j]].w = 0;
                    for (int m = i; first_state.map[m][j] == block[first_state.map[i][j]].code; m--)
                    {
                        block[first_state.map[i][j]].h++;
                    }
                    for (int n = j; first_state.map[i][n] == block[first_state.map[i][j]].code; n++)
                    {
                        block[first_state.map[i][j]].w++;
                    }
                    num[first_state.map[i][j]] = -1;
                }
        }
    }
    printf("地图：\r\n");
    for (int i = map_y - 1; i >= 0; i--)
    {
        for (int j = 0; j < map_x; j++)
        {
            if (first_state.map[i][j] != MAXINIT)
                printf(" %d", first_state.map[i][j]);
            else
                printf(" +");
        }
        printf("\r\n");
    }
    printf("\r\n");
    printf("目标地址：\r\n");
    scanf("%d %d", &tar_x, &tar_y);
    fclose(fp);
}
/*输出到txt*/
void print2(Solution *h)
{
    FILE *fp2, *fp;
    char ch;
    char oo[5];
    fp = fopen("step.txt", "w");
    if (fp2 == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }

    Solution *p = h;
    while (h != NULL)
    {
        for (int i = map_y - 1; i >= 0; i--)
        {
            for (int j = 0; j < map_x; j++)
            {
                if (h->map[i][j] != MAXINIT)
                {
                    oo[0] = ' ';
                    fprintf(fp, oo);
                    oo[0] = (char)(h->map[i][j] + 48);
                    fprintf(fp, oo);
                }
                else if (h->map[i][j] == MAXINIT)
                {
                    oo[0] = ' ';
                    fprintf(fp, oo);
                    oo[0] = '+';
                    fprintf(fp, oo);
                }
            }
            oo[0] = '\n';
            fprintf(fp, oo);
        }
        h = h->father;
        oo[0] = '\n';
        fprintf(fp, oo);
        fprintf(fp, oo);
        fprintf(fp, oo);
    }

    fclose(fp);
}