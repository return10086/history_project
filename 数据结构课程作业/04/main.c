#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
/*******ͼ���ڽӾ����ʾ***********/
#define MaxInt 32767 // ��ʾ����ֵ
#define MVNum 100    // ��󶥵���

typedef struct data
{
    char *name;     // ����������
    char code_name; // ����
    char *p;        // ���
} data;             // ������ݽṹ��

typedef struct
{
    data spot; // ����ص�
    char code; // �������
} VerTexType;  // ������Ϣ�ṹ��

typedef struct
{
    VerTexType vexs[MVNum]; // �����
    int arcs[MVNum][MVNum]; // �ڽӾ���
    int vexnum, arcnum;     // ͼ�ĵ�ǰ�����ͱ���
} AMGraph;

// DFS�㷨
int path[MVNum];
int pathNumber = 0;
int visited[MVNum];
int count = 0;
int min_weight = 100;
int ww = 0;

int D[MVNum]; // �Ͻ�˹�����㷨������¼���·��

// ����������
void CreateUDN(AMGraph *G);
// DFS������ȱ���
int DFS(AMGraph *G, int nowPoint, int count, int endPoint, int **short_way, int min_weight);
// �Ͻ�˹�����㷨
void ShortTestPath_DIJ(AMGraph *G, int v0);
int next(AMGraph *G, int n, int now);
int first(AMGraph *G, int n);
// ��·
void close_way(AMGraph *G, int point1, int point2);
// ��·
void open_way(AMGraph *G, int point1, int point2, int weight);
// ��ȡ����
void read_txt(const char *txt);
// д�ĵ�
void write_txt(AMGraph *G, char *content, char *spot, int num);
// ƴ���ַ���
char *add_together(char *s1, char *s2);
// �ж���������û�������
int adjust(int *m, int a);
int main()
{
    AMGraph G;
    CreateUDN(&G);
    while (1)
    {
        printf("*************���Ƽ���ѧ����ָ��******************\r\n");
        printf("����̨\r\n");
        printf("(1)��ͼȫ��\r\n");
        printf("(2)·���滮\r\n");
        printf("(3)����ָ��\r\n");
        printf("(4)������·\r\n");
        printf("(5)���Ӿ������\r\n");
        int c;
        scanf("%d", &c);
        switch (c)
        {
        case 1:
        {
            read_txt("map.txt");
            printf("\r\n");
        }
        break;
        case 2:          // ·���滮
        {                //
            int tar[20]; // ��������ʮ��Ŀ��ص�
            int pos;     // ��ǰλ��
            int cc = 0;  // ��¼Ŀ����Ŀ
            int t;       // ���뻺����
            printf("����������λ��\r\n");
            scanf("%d", &pos);
            printf("����������Ŀ�ĵ�(���븺������)\r\n");
            while (1)
            {
                scanf("%d", &t);
                if (t < 0)
                    break;
                else
                    tar[cc++] = t;
            }
            // ��ʱ�Ѽ����û���Ϣ
            for (int i = cc - 1; i >= 0; i--)                                    // ������Ŀ�����ΪĿ��
            {                                                                    //
                int **short_way;                                                 // ��������ͨ��Ŀ���·������һλ��Ȩֵ��
                short_way = (int **)malloc(sizeof(int *) * 20);                  // ��άָ�����ռ�
                count = 0;                                                       // DFS�ݹ��㷨�õ���ȫ�ֱ�����λ
                pathNumber = 0;                                                  // DFS�ݹ��㷨�õ���ȫ�ֱ�����λ
                int number = DFS(&G, pos, count, tar[i], short_way, min_weight); // number����鵽·����Ŀ
                min_weight = 1000;                                               // DFS�ݹ��㷨�õ���ȫ�ֱ�����λ
                ww = 0;                                                          // DFS�ݹ��㷨�õ���ȫ�ֱ�����λ
                for (int l = number - 1; l >= 0; l--)                            // ���·������Ŀ��ʹ�ӡ
                {                                                                //
                    int n = 1;                                                   // ��־λ�������ж��Ƿ��������Ŀ��
                    for (int t = cc - 1; t >= 0; t--)                            // һ��Ԫ���ж�һ��
                    {                                                            //
                        if (adjust(short_way[l], tar[t]) > 0)                    // ��������Ԫ��
                            ;                                                    // ����
                        else                                                     // ���û�����Ԫ��
                            n = 0;                                               // �ѱ�־λ��0
                    }                                                            //
                    if (n)                                                       // �����������Ŀ���
                    {                                                            // ��ӡ��һ��·��
                        printf("�Ƽ�·������Ϊ��%d\r\n", short_way[l][0]);          // ��һλ��Ȩֵ
                        for (int h = 0; short_way[l][h] != tar[i]; h++)          //
                        {                                                        //
                            if (short_way[l][h + 1] != tar[i])                   //
                                printf("%d->", short_way[l][h + 1]);             //
                            else                                                 //
                                printf("%d", short_way[l][h + 1]);               //
                        }                                                        //
                        printf("\r\n");                                          //
                    }                                                            //
                }                                                                //
            }                                                                    //
        }                                                                        //
        break;

        case 3:                                    // ����ָ��
        {                                          //
            printf("�����˽��ĸ�����");              //
            int t;                                 // �������
            char q[5];                             // ���ַ���
            scanf("%d", &t);                       //
            itoa(t, q, 10);                        // ���������תΪ�ַ���
            read_txt(G.vexs[t].spot.p);            // ��ӡ���������
            printf("\r\n");                        //
            char *final = add_together(q, ".jpg"); // ������Ϊ�������+.jpg��ͼƬ
            char cmd[100];                         //
            sprintf(cmd, "mspaint %s", final);     // ��mspain
            system(cmd);                           // ��ͼƬ
        }
        break;
        case 4:
        {
            printf("a.��·ά����  b.��·�������\r\n");
            char t;
            scanf("%c", &t);
            printf("������·��\r\n");
            int s1, s2;
            scanf("%d,%d", &s1, &s2);
            switch (t)
            {
            case 'a':
                close_way(&G, s1, s2);
                break;
            case 'b':
                printf("��·���ȣ�\r\n");
                int weig;
                scanf("%d", &weig);
                open_way(&G, s1, s2, weig);
            default:
                break;
            }
        }
        break;
        case 5:
        {
            printf("�����ĸ�����Ľ��ܣ�");
            char q[5];
            scanf("%s", &q);
            int qq;
            qq = atoi(q);
            printf("�����ӽ���");
            char c[MaxInt];
            scanf("%s", &c);
            char *final = add_together(q, "_txt");
            write_txt(&G, c, final, qq);
        }
        break;
        default:
            break;
        }
    }

    system("pause");
}
// ����������
void CreateUDN(AMGraph *G)
{
    printf("�����ܶ��������ܱ���");
    scanf("%d,%d", &G->vexnum, &G->arcnum); // �����ܶ��������ܱ���
    for (int i = 0; i < G->vexnum; ++i)
    {
        for (int j = 0; j < G->vexnum; ++j)
        {
            G->arcs[i][j] = MaxInt;
        }
        G->vexs[i].code = i; // ��������
    }
    int v1, v2, weight;

    for (int k = 0; k < G->arcnum; ++k)
    {
        printf("������������,�ߵ�Ȩֵ:");
        scanf("%d,%d,%d", &v1, &v2, &weight); // ����һ���������Ķ����Ȩֵ
        G->arcs[v1][v2] = weight;
        G->arcs[v2][v1] = weight;
    }
}

// ���·��
void ShortTestPath_DIJ(AMGraph *G, int v0)
{
    int n = G->vexnum;
    int S[MVNum];    // ��¼��С����
    int Path[MVNum]; // ֱ��ǰȥ������𩣬�л�Ϊv0,�޻�Ϊ-1

    for (int v = 0; v < n; ++v)
    {
        S[v] = 0;
        D[v] = G->arcs[v0][v];
        if (D[v] < MaxInt)
        {
            Path[v] = v0;
        }
        else
        {
            Path[v] = -1;
        }
    }
    S[0] = 1;
    D[v0] = 0;
    int v;
    for (int i = 1; i < n; ++i)
    {
        int min = MaxInt;
        for (int w = 0; w < n; ++w)
            if (!S[w] && D[w] < min)
            {
                v = w;
                min = D[w];
            }
        S[v] = 1;
        for (int w = 0; w < n; ++w)
            if (!S[w] && (D[v] + G->arcs[v][w] < D[w]))
            {
                D[w] = D[v] + G->arcs[v][w];
                Path[w] = v;
            }
    }
}

// DFSѰ������·��
// countΪ����㵽��ǰ�ڵ㾭���Ľڵ���,pathNumberΪѰ�ҵ���·������
int DFS(AMGraph *G, int nowPoint, int count, int endPoint, int **short_way, int min_weight)
{
    // char short_way[100]; //�������·��
    // int min_weight;
    visited[nowPoint] = 1;
    path[count++] = nowPoint;

    if (nowPoint == endPoint && (count) >= 1)
    {
        pathNumber++;
        int weight = 0;
        // printf("���������м��%d����·��Ϊ: ", pathNumber);
        for (int i = 0; i < (count)-1; i++)
        {
            // printf("%d-->", path[i]);
        }
        // printf("%d", path[(count)-1]);
        for (int j = 0; j < count - 1; j++)
        {
            weight = weight + G->arcs[path[j]][path[j + 1]];
        }
        // printf("��·��Ϊ��%d\r\n", weight);
        short_way[ww] = (int *)malloc(sizeof(int) * (100));
        short_way[ww][0] = weight;
        for (int k = 1; k <= count;)
        {
            short_way[ww][k++] = path[k - 1];
        }
        ww++;
    }
    else
    {
        for (int k = first(G, nowPoint); k < G->vexnum; k = next(G, nowPoint, k))
        {
            if (visited[k] == 0)
                DFS(G, k, count, endPoint, short_way, min_weight);
        }
    }
    visited[nowPoint] = 0; // �㷨�ؼ������ݣ������ʹ�״̬��Ϊδ����״̬
    (count)--;
    return pathNumber;
}

// ���ص�n���ڵ������������С�Ľڵ�
int first(AMGraph *G, int n)
{
    for (int i = 0; i < G->vexnum; i++)
    {
        if (G->arcs[n][i] != MaxInt)
            return i;
    }
    printf("���Ϊ���ϵ㣬������һ����Ϊ���ϵ�����\n");
    return 0;
}

// ���ص�n���ڵ������ı�now�ڵ���Ŵ�Ľڵ�,
int next(AMGraph *G, int n, int now)
{
    for (int i = now + 1; i < G->vexnum; i++)
    {
        if (G->arcs[n][i] != MaxInt)
            return i;
    }
    return 1000; // ���Ҳ����ڵ�n����һ���ڵ㣬����1000
}

// ��·
void close_way(AMGraph *G, int point1, int point2)
{
    G->arcs[point1][point2] = MaxInt;
    G->arcs[point2][point1] = MaxInt;
}

// ��·
void open_way(AMGraph *G, int point1, int point2, int weight)
{
    G->arcs[point1][point2] = weight;
    G->arcs[point2][point1] = weight;
}

// ��ȡ����
void read_txt(const char *txt)
{
    FILE *fp;
    char ch;
    fp = fopen(txt, "r");
    if (fp == NULL)
    {
        printf("�þ������޼��\n");
    }
    ch = fgetc(fp);
    while (ch != -1)
    {
        printf("%c", ch);
        ch = fgetc(fp);
    }
    fclose(fp);
}

void write_txt(AMGraph *G, char *content, char *spot, int num)
{
    FILE *fp;
    char ch;
    G->vexs[num].spot.p = spot;
    fp = fopen(spot, "w");
    ch = *content;
    char oo[2];
    while (ch != '\000')
    {
        oo[0] = ch;
        fprintf(fp, oo);
        content += 1;
        ch = *content;
    }

    fclose(fp);
}

// ƴ���ַ���
char *add_together(char *s1, char *s2)
{
    char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1);
    if (result == NULL)
        exit(1);

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

// �ж���������û�������
int adjust(int *m, int a)
{
    int c = 0;
    while (m[c] >= 0 && (m[c]) < 100) // ��������
    {
        c++;
        if (m[c] == a)
            return 1;
    }
    return -1;
}