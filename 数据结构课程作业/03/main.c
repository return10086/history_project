#include "stdlib.h"
#include "stdio.h"
#include "string.h"

typedef struct
{
    char letter;       //代表字符
    int weight;        //权重
    char Huffman_code; //编码
} alphabet;

typedef struct Tree_point
{
    alphabet word;
    struct Tree_point *point_father;     //父指针
    struct Tree_point *point_left_child; //左孩子指针
    struct Tree_point *point_right;      //右指针
    char num;                            //用于判断父节点是否存在，存在写1
} Tree_point;                            //二叉树结点

typedef struct List
{
    Tree_point data;   //数据域
    struct List *next; //指向下一个结点
} List;

/**************变量定义***************/
char *code[256]; //储存编码的字符串数组

/***************函数声明**************/
//创建表头
List *creathead(void);
//创建结点
List *creatList(List *a, char letter);
//插入结点（在表头之后）（头插法）
List *insertList(List *headList, char letter);
//插入结点（带权值的结点插在有序链表）
List *insertList2(List *headList, List *point);
//链表排序
void rank(List *L);
//初始化非最底下的结点
List *init(List *left, List *right);
//读取文章，创建结点
void read_txt(List *L);
//判断L里有没有a这个元素
List *adjust(List *headList, char a);
//链表长度
int getLen(List *L);
//从队列L取第num个结点转移信息到队列L2
void transfer(List *L, int num, List *L2);
//打印编码规则
void print(List *L2);
void txt_to_code(List *L);
void code_to_txt(Tree_point *p);
int main()
{

    List *L = creathead();
    List *L2 = creathead();
    read_txt(L);
    rank(L);
    while (getLen(L) != 1)
    {

        List *m = init(L->next, L->next->next);
        transfer(L, 1, L2);
        transfer(L, 1, L2);
        insertList2(L, m);
    }
    print(L2);
    txt_to_code(L);
    printf("\r\n");
    code_to_txt(&L->next->data);
    system("pause");
}

/***************函数定义**************/
List *init(List *left, List *right)
{
    List *father = (List *)malloc(sizeof(List));
    father->data.point_left_child = &left->data;

    left->data.point_father = &father->data;
    right->data.point_father = &father->data;

    left->data.point_right = &right->data;
    right->data.point_right = NULL;

    left->data.num = 1;
    right->data.num = 1;

    left->data.word.Huffman_code = '0';
    right->data.word.Huffman_code = '1';

    father->data.word.weight = left->data.word.weight + right->data.word.weight;
    return father;
}

//创建链表（表头）
List *creathead(void)
{
    List *headList = (List *)malloc(sizeof(List));
    headList->next = NULL;
    return headList;
}

//创建结点
List *creatList(List *L, char letter)
{
    //开辟节点空间
    List *a = (List *)malloc(sizeof(List));
    a->data.word.letter = letter;
    return a;
}

//插入结点(在表头和第一个结点之间）
List *insertList(List *headList, char letter)
{
    //创建要插入的结点
    List *NewList = creatList(headList, letter);
    //重新建立关系
    NewList->next = headList->next;
    headList->next = NewList;
    return NewList;
}

//插入结点（带权值的结点插在有序链表）
List *insertList2(List *headList, List *point)
{
    List *p = headList;
    if (p->next != NULL)
    {
        while (p->next->data.word.weight < point->data.word.weight)
        {

            p = p->next;
            if (p->next == NULL)
                break;
        }
    }
    point->next = p->next;
    p->next = point;
}
//交换结点的，对接收时间从小到大的排序
void rank(List *L)
{
    if ((L->next == NULL) || (L->next->next == NULL))
    {
        return;
    }
    List *prep; // prep前一项 curp当前项 nextp后一项 end控制循环
    List *nextp;
    List *end = NULL; //最开始一轮是所有的元素都要比较，所以end最开始为空
    List *curp;
    while (L->next != end) //冒泡排序都是双重循环，这里为外循环用于控制
    {                      //循环的趟数
        //初始化三个指针 ; 判断是否到达结束位置 ; 三个指针集体后移
        for (prep = L, curp = prep->next, nextp = curp->next; nextp != end; prep = prep->next, curp = curp->next, nextp = nextp->next)
        {
            if (curp->data.word.weight > nextp->data.word.weight) //从小到大
            {
                prep->next = nextp; //交换指针指向
                curp->next = nextp->next;
                nextp->next = curp;
                //此时next变前一项，cur变后一项  交换next cur
                List *temp = curp;
                curp = nextp;
                nextp = temp;
            }
        }
        //一轮循环结束 最后一项已经排好 end提前一项 (冒泡原理)
        end = curp; //此时curp就是倒数第i项。i表示内循环执行的次数
    }
}

//读取文章
void read_txt(List *L)
{
    FILE *fp;
    char ch;
    fp = fopen("test.txt", "r");
    if (fp == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }
    ch = fgetc(fp);
    while (ch != -1)
    {
        List *T = adjust(L, ch);
        if (T == NULL) //如果没有这个元素
        {
            List *m = insertList(L, ch); //创建结点并插入
            m->data.word.weight = 1;
        }
        else
        {
            T->data.word.weight++;
        }
        ch = fgetc(fp);
    }
    fclose(fp);
}

//判断L里有没有a这个元素
List *adjust(List *L, char a)
{
    List *p = L->next;
    int lenth = getLen(L);
    for (lenth; lenth > 0; lenth--)
    {
        if (p->data.word.letter != a) //如果链表元素里没有a
        {
            p = p->next;
        }
        else //如果有a
        {
            return p; //返回a的结点指针
        }
    }
    return NULL; //如果没有这个元素返回NULL
}

int getLen(List *L)
{
    if (L->next == NULL)
        return 0; //头指针指针域为空，说明单链表不含任何元素
    int len = 0;
    List *r = L->next;
    while (r)
    {
        r = r->next; //尾指针后移
        len++;
    }
    return len;
}

//从队列L取第num个结点转移信息到队列L2
void transfer(List *L, int num, List *L2)
{
    List *point = L->next;
    num -= 1;
    while (num--)
    {
        point = point->next;
        L = L->next;
    }
    L->next = point->next;
    point->next = L2->next;
    L2->next = point;
}

//打印最终全部编码
void print(List *L2)
{
    FILE *fp;
    char ch;
    char oo[20];
    fp = fopen("code.txt", "w");
    if (fp == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }

    char number[20];
    int num = 0;
    int m = 0;
    List *p = L2->next; //遍历结点
    while (1)
    {
        if (p->data.word.letter != '\r') //如果这个点是储存数据的结点
        {

            Tree_point *q = &p->data; //读取数据
            char a = q->word.letter;
            printf("字符:   %c 权值：%d    编码：", q->word.letter, q->word.weight);

            oo[0] = q->word.letter;
            // printf("%s,%c", oo, q->word.letter);
            fprintf(fp, oo); //把编码写到文件里
            char *qq = ":";
            fprintf(fp, qq);
            while (1) //打印编码
            {
                if (q->num == 1) //如果有父节点
                {
                    number[num++] = q->word.Huffman_code;
                    q = q->point_father; //就继续读父节点
                }
                else //如果没有父节点
                {
                    break;
                }
            }
            int c = num;
            code[(int)a] = (char *)malloc(num);
            char *s = code[(int)a];
            for (num; num > 0; num--)
            {
                *s = number[num - 1];
                printf("%c", number[num - 1]);
                oo[0] = number[num - 1];
                fprintf(fp, oo); //把编码写到文件里
                s = s + 1;
            }

            // printf("%s", s - c);
            // fprintf(fp, s - c); //把编码写到文件里

            printf("\r\n");
            oo[0] = '\r';
            fprintf(fp, oo);
            if (p->next != NULL) //如果 L2后面还有数据
                p = p->next;
            else //如果没数据了
                break;
        }
        else                     //不是储存数据的结点
            if (p->next != NULL) //如果L2后面还有数据
                p = p->next;
    }
    fclose(fp);
}

// txt->code
void txt_to_code(List *L)
{
    FILE *fp, *fp2;
    char ch;
    fp = fopen("test.txt", "r");
    fp2 = fopen("Huffman_code.txt", "w");
    if (fp == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }
    ch = fgetc(fp);
    char *a;
    char oo[2];
    while (ch != -1)
    {
        char *p = code[(int)ch];
        int j = 0;
        do
        {
            printf("%c", *(p + j));
            if (*(p + j) == '1')
            {
                oo[0] = '1';
            }
            else
            {
                oo[0] = '0';
            }
            fprintf(fp2, oo);
            j++;
        } while (*(p + j) == '0' || *(p + j) == '1');
        ch = fgetc(fp);
        j = 0;
    }

    fclose(fp);
    fclose(fp2);
}

// code->txt
void code_to_txt(Tree_point *p)
{
    FILE *fp2, *fp;
    char ch;
    char oo[20];
    fp = fopen("test2.txt", "w");
    fp2 = fopen("Huffman_code.txt", "r");
    if (fp2 == NULL)
    {
        printf("Failure to open demo.text!\n");
        exit(0);
    }
    ch = fgetc(fp2);
    Tree_point *q = (Tree_point *)malloc(sizeof(Tree_point));
    q = p;
    while (ch != -1)
    {
        switch (ch)
        {
        case '0':
            q = q->point_left_child;

            break;

        default:
            q = q->point_left_child->point_right;
            break;
        }
        if (q->word.letter != '\r')
        {
            printf("%c", q->word.letter);
            oo[0] = q->word.letter;
            fprintf(fp, oo); //把编码写到文件里
            q = p;
        }
        ch = fgetc(fp2);
    }
    fclose(fp);
    fclose(fp2);
}