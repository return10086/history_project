#include "stdlib.h"
#include "stdio.h"
/*******************************singelList.h**********************************************/
typedef struct mail
{
    char *name;          //对方的昵称（最大20个字符）
    int time;            //信息发来时间
    const char *content; //信息内容
} mail;

typedef struct List
{
    mail data;  //数据域
    List *next; //指向下一个结点
} List;

//创建表头
List *creathead(void);
//创建结点
List *creatList(mail *data, char (*WriteData)(mail *, char *, int, const char *), char *name, int time, const char *content);
//插入结点（在表头之后）（头插法）
void insertList1(List *headList, mail *data, char *name, int time, const char *content);
//尾插法
void insertList2(List *headList, mail *data, char *name, int time, const char *content);
//打印第几个结点内容（不包括表头）
List *printpoint(List *L, int num);
//按信息接收时间排序
void rank(List *L);
//获取链表长度
int getLen(List *L);
//拼接链表,并把拼接好的链表放进第一个链表
void together(List *A, List *B);
//删除信息
void Delete(List *L, int num);
//在指定位置插入结点(某个结点后）（headList后，where写0）
void insertList3(List *headList, int where, mail *data, char *name, int time, const char *content);
//检索字符串（包括部分字符串）
void search(List *L, const char *a, int t);

/**************上面是各种链表函数声明************/

/***********下面是只针对mail进行操作的函数**********/
//向结构体里写数据
char WriteData(mail *M, char *name, int time, const char *content);
//创建一个信息结构体
mail *creatMail(void);

/*******************************singelList.h**********************************************/

char name1[] = "程佳乐";

int main()
{
    List *L1 = creathead(); //创建表头
    List *L2 = creathead(); //创建第二个表头

    //头插法
    mail *M1 = creatMail();
    insertList1(L1, M1, name1, 100, "message1");
    mail *M2 = creatMail();
    insertList1(L1, M2, name1, 120, "message2");
    mail *M3 = creatMail();
    insertList1(L1, M3, name1, 140, "message3");
    mail *M4 = creatMail();
    insertList1(L1, M4, name1, 160, "message4");
    mail *M5 = creatMail();
    insertList1(L1, M5, name1, 180, "message5");
    mail *M6 = creatMail();
    insertList1(L1, M6, name1, 200, "message6");
    //尾插法
    mail *M7 = creatMail();
    insertList2(L1, M7, name1, 220, "message7");
    // L1真实的排序是：headList,6，5，4，3，2，1，7

    rank(L1); //把L1根据接收时间进行从小到大的排序
    // L1真实的排序是：headList,1，2，3，4，5，6，7

    printf("/****************排序后查看结点1*************************/\r\n");
    List *p = printpoint(L1, 1); //查看结点1

    //头插法
    mail *M8 = creatMail();
    insertList1(L2, M8, name1, 240, "message8");
    mail *M9 = creatMail();
    insertList1(L2, M9, name1, 260, "message9");
    mail *M10 = creatMail();
    insertList1(L2, M10, name1, 280, "message10");
    mail *M11 = creatMail();
    insertList1(L2, M11, name1, 300, "message11");
    //尾插法
    mail *M12 = creatMail();
    insertList2(L2, M12, name1, 320, "message12");
    //所以L2真实的排序是:headList,11,10,9,8,12

    together(L1, L2); //拼接两个链表
    //所以L1真实的排序是：headList，1，2，3，4，5，6，7，11，10，9，8，12

    printf("/****************查看第10个结点*************************/\r\n");
    p = printpoint(L1, 10);

    //再排序
    rank(L1);
    //所以L1真实的排序是：headList，1，2，3，4，5，6，7，8，9，10，11，12

    printf("/****************排序后再查看第10个结点*************************/\r\n");
    p = printpoint(L1, 10);

    Delete(L1, 1); //删掉第一个

    printf("/****************删掉第一个结点后查看第一个结点*************************/\r\n");
    p = printpoint(L1, 1);
    //所以L1真实的排序是：headList，2，3，4，5，6，7，8，9，10，11，12

    insertList3(L1, 0, M1, name1, 100, "measage1"); //把第一个结点添回来

    printf("/****************补回第一个结点后查看第一个结点*************************/\r\n");
    p = printpoint(L1, 1);

    printf("/*******************************************/\r\n");
    search(L1, "ess", 3); //检索ess字符，只有第一个不对
    system("pause");
    return 0;
}

/*******************************singelList.c**********************************************/
//创建链表（表头）
List *creathead(void)
{
    //开辟空间
    List *headList = (List *)malloc(sizeof(List));
    //初始化数据
    if (headList != NULL) //如果没有这个会触发空间不足警告
        headList->next = NULL;
    return headList;
}

//创建结点,(哪个链表，链表的写入函数，姓名，时间，信息内容）
List *creatList(mail *data, char (*WriteData)(mail *, char *, int, const char *), char *name, int time, const char *content)
{
    //开辟节点空间
    List *L = (List *)malloc(sizeof(List));
    //初始化数据
    WriteData(data, name, time, content);
    if (L != NULL) //如果没有这个会触发空间不足警告
        L->data = *data;
    if (L != NULL) //如果没有这个会触发空间不足警告
        L->next = NULL;
    return L;
}

//插入结点(在表头和第一个结点之间）（头插法）
void insertList1(List *headList, mail *data, char *name, int time, const char *content)
{
    //创建要插入的结点
    List *NewList = creatList(data, WriteData, name, time, content);
    //重新建立关系
    NewList->next = headList->next;
    headList->next = NewList;
}

//尾插法
void insertList2(List *headList, mail *data, char *name, int time, const char *content)
{
    //创建要插入的结点
    List *NewList = creatList(data, WriteData, name, time, content);

    int num = getLen(headList);
    List *p = headList->next;
    num = num - 1;
    while (num--)
    {
        p = p->next;
    }
    p->next = NewList;
}
//打印第几个结点内容（不包括表头）
List *printpoint(List *L, int num)
{
    List *p = L;
    while (num--)
    {
        p = p->next; //遍历结点移位
    }
    printf("name=%s\r\n", p->data.name);
    printf("time=%d\r\n", p->data.time);
    printf("content=%s\r\n", p->data.content);
    printf("\r\n");
    return p;
}

//交换结点的，对接收时间从小到大的排序
void rank(List *L) //这玩意太好玩了:-(
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
            if (curp->data.time > nextp->data.time) //从小到大
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

//拼接链表,并把拼接好的链表放进第一个链表
void together(List *A, List *B)
{
    int num = getLen(A);
    List *p = A->next;
    num = num - 1;
    while (num--)
    {
        p = p->next;
    }
    p->next = B->next;
}

//删除信息
void Delete(List *L, int num)
{
    List *point = L->next;
    num -= 1;
    while (num--)
    {
        point = point->next;
        L = L->next;
    }
    L->next = point->next;
    free(point); //释放空间
}
//检索字符串(哪个链表，检索什么，几个字符相连就打印出来）
void search(List *L, const char *a, int t)
{
    const char *b = a;
    List *p = L->next;
    const char *d = p->data.content;
    int num = getLen(L);
    int l = num;
    for (; num > 0; num--) //第一层循环
    {
        int n = 0;
        char m[20]; //定义一个字符串储存这一次比较中相同的字符串的内容
        char *q;
        q = m;
        char flag = 0;
        int lenth = sizeof(p->data.content); //获取结点信息长度

        for (; lenth > 0; lenth--) //逐个字符比较
        {
            if (*a == *d) //如果这个字符相同
            {
                n = n + 1; //计数有几个字符相同
                *q = *a;   //把这个字符写到字符串
                q = q + 1; //字符串向后移位
                a++;       //看下一个字符是不是一样
                d++;       //看下一个字符是不是一样
                flag = 1;  //告诉下一次，这次的字符判断相同
                           //继续判断下一个字符
            }
            else
            {
                if (flag == 0) //如果上一个字符不相同
                {
                    d++; //看下一个字符
                }
                else //上一个字符相同
                {
                    if (n >= t) //如果相同的字符够数了
                    {
                        List *w = printpoint(L, l - num + 1); //把符合条件的结点找出来
                    }
                    n = 0;
                    //开始写下一个字符串数组
                }
            }
        }
        if (p != NULL) //如果没有这个会触发空间不足警告
            p = p->next;
        if (p != nullptr)
        {
            d = p->data.content;
            a = b;
        }
    }
}

char WriteData(mail *M, char *name, int time, const char *content)
{
    M->content = content;
    M->name = name;
    M->time = time;
    return 1;
}

mail *creatMail(void)
{
    mail *M = (mail *)malloc(sizeof(mail));
    return M;
}

//在指定位置插入结点(在第几个后）（headList为0）
void insertList3(List *headList, int where, mail *data, char *name, int time, const char *content)
{
    if (where == 0)
    {
        insertList1(headList, data, name, time, content);
    }
    else
    {
        List *NewList = creatList(data, WriteData, name, time, content);
        List *p = headList->next;
        where = where - 1;
        while (where--)
        {
            p = p->next;
        }

        List *L = p->next;
        p->next = NewList;
        NewList->next = L;
    }
}

/******************************singleList****************************/