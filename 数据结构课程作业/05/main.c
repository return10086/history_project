#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
typedef struct
{
    int key; //代表字符
    //添加其他数据项
} Elemtype;

typedef struct Tree_point
{
    Elemtype *data;                       //数据域
    struct Tree_point *point_father;      //父节点
    struct Tree_point *point_left_child;  //左孩子指针
    struct Tree_point *point_right_child; //右孩子指针
    int level;                            //层次
    int order;                            //所在行顺序
} Tree_point;                             //二叉树结点

int maxlevel;        //最大层次
int count_level = 0; //用作添加结点时计数

//用作打印二叉树
Tree_point ***Tree;                            //二维数组，数组中储存的是指针
int *Tree_count;                               //记录每一层元素个数
Tree_point example = {NULL, NULL, NULL, 0, 0}; //令分配好空间的Tree全部初始化指向example，便于判断是否为空

//中序排序找某点的下一点的标志位
int next_point_flag = 1;

//创建根结点
Tree_point *creat_root_point(int key);
//创建结点
void creat_point(Tree_point *root_point, int key, double count);
//查找结点
Tree_point *search_tree_point(Tree_point *root_point, int key);
//删除结点
void Delete(Tree_point *root_point, int key);
//打印二叉树
void print_Tree(Tree_point *root_point);
//判断位数
int adjust(int num);
//中序遍历
void inorder_traversal(Tree_point *root_point);
//找到某点中序遍历中的下一个点
Tree_point *next_point(Tree_point *root_point, Tree_point *some_point);
int main()
{
    Tree = (Tree_point ***)malloc(sizeof(Tree_point **) * 20);               //初始化第一层指针数组
    Tree_count = (int *)malloc(sizeof(int) * 20);                            //最多存20层
    Tree[0] = (Tree_point **)malloc(sizeof(Tree_point *) * (pow(2, 0) + 1)); //第一层初始化
    int head_key;
    printf("请输入头节点");
    scanf("%d", &head_key);
    Tree_point *T = creat_root_point(head_key);
    while (1)
    {

        printf("1、删除元素 2、添加元素 3、打印二叉树 4、查找元素 5、中序遍历 6、替换结点\r\n");
        int a;
        scanf("%d", &a);
        int b;
        switch (a)
        {
        case 1:
            printf("请输入要删除的元素\r\n");
            scanf("%d", &b);
            Delete(T, b);
            break;
        case 2:
            printf("输入结点\r\n");
            scanf("%d", &b);
            creat_point(T, b, 1);
            break;
        case 3:
            print_Tree(T);
            break;
        case 4:
            printf("您查找元素的方式:(1)关键字查询  (2)行位查询\r\n");
            int e;
            scanf("%d", &e);
            if (e == 1)
            {
                printf("请输入关键字\r\n");
                int d;
                scanf("%d", &d);
                Tree_point *L = search_tree_point(T, d);
                if (L == NULL)
                    printf("查找失败，不含这个元素\r\n");
                else
                    printf("查找成功,层：%d,位数：%d\r\n", L->level, L->order); //补
            }
            else if (e == 2)
            {
                printf("请输入元素所在层位\r\n");
                int f, g;
                scanf("%d,%d", &f, &g);
                int h = Tree[f][g]->data->key;
                if (h > 0)
                {
                    printf("元素为%d\r\n", h);
                }
                else
                {
                    printf("没找到元素\r\n");
                }
            }
            break;
        case 5:
            inorder_traversal(T);
            printf("\r\n");
            break;
        case 6:
        {
            int t1, t2;
            printf("您要替换的元素是");
            scanf("%d", &t1);
            printf("您要替换为：");
            scanf("%d", &t2);
            printf("\r\n");
            Tree_point *L = search_tree_point(T, t1);
            if (L == NULL)
                printf("查找失败，不含这个元素\r\n");
            else
            {
                L->data->key = t2;
                printf("替换成功");
            }
        }
        break;
        default:
            break;
        }
    }
    system("pause");
}
//创建根结点
Tree_point *creat_root_point(int key)
{
    Tree_point *h = (Tree_point *)malloc(sizeof(Tree_point));
    h->data = (Elemtype *)malloc(sizeof(Elemtype));
    h->data->key = key;
    h->point_left_child = NULL;
    h->point_right_child = NULL;
    h->point_father = NULL;
    h->level = 0; //根节点层次为0
    h->order = 0;
    Tree[0][0] = h;
    Tree_count[0] = 1;
    return h;
} //创建结点
void creat_point(Tree_point *root_point, int key, double count)
{
    if (key > root_point->data->key) //向右走
    {
        count_level += 1; //用于计算新添加元素在第几层
        count = count;    //用于计算最终叶子结点所排位数
        if (root_point->point_right_child == NULL)
        {
            root_point->point_right_child = (Tree_point *)malloc(sizeof(Tree_point));                        //分配新结点空间
            root_point->point_right_child->data = (Elemtype *)malloc(sizeof(Elemtype));                      //数据用指针在结构体定义，此处应分配空间
            root_point->point_right_child->data->key = key;                                                  //写数据
            root_point->point_right_child->point_left_child = NULL;                                          //左节点初始化
            root_point->point_right_child->point_right_child = NULL;                                         //右结点初始化
            root_point->point_right_child->point_father = root_point;                                        //父节点初始化
            if (count_level > maxlevel)                                                                      //如果这是新开一层
            {                                                                                                //
                Tree[count_level] = (Tree_point **)malloc(sizeof(Tree_point *) * (pow(2, count_level) + 1)); //初始化新的一层
                for (int i = pow(2, count_level); i >= 0; i--)                                               //把Tree数组初始化指向example
                    Tree[count_level][i] = &example;                                                         //
                maxlevel = count_level;                                                                      //改最大层数
                Tree_count[count_level] = 0;                                                                 //开始计数
            }
            root_point->point_right_child->order = root_point->order * 2 + 1;                          //左节点的位数等于母结点位数*2+1
            root_point->point_right_child->level = count_level;                                        //写层数
            Tree_count[count_level]++;                                                                 //这一层多一个元素
            Tree[count_level][(int)(count * pow(2, count_level)) - 1] = root_point->point_right_child; // Tree指向此元素
            count_level = 0;                                                                           //恢复全局变量
            printf("创建完成\r\n");
            return;
        }
        else
        {
            creat_point(root_point->point_right_child, key, count);
        }
    }
    else if (key < root_point->data->key) //往左走
    {
        count_level += 1;                                  //用于计算新添加元素在第几层
        count = count - pow(0.5, root_point->level) * 0.5; //用于计算最终叶子结点所排位数

        if (root_point->point_left_child == NULL)
        {
            root_point->point_left_child = (Tree_point *)malloc(sizeof(Tree_point));                         //分配新结点空间
            root_point->point_left_child->data = (Elemtype *)malloc(sizeof(Elemtype));                       //数据用指针在结构体定义，此处应分配空间
            root_point->point_left_child->data->key = key;                                                   //写数据
            root_point->point_left_child->point_left_child = NULL;                                           //左节点初始化
            root_point->point_left_child->point_right_child = NULL;                                          //右结点初始化
            root_point->point_left_child->point_father = root_point;                                         //父节点初始化
            if (count_level > maxlevel)                                                                      //如果这是新开一层
            {                                                                                                //
                Tree[count_level] = (Tree_point **)malloc(sizeof(Tree_point *) * (pow(2, count_level) + 1)); //初始化新的一层
                for (int i = pow(2, count_level); i >= 0; i--)                                               //把Tree数组初始化指向example
                    Tree[count_level][i] = &example;                                                         //
                maxlevel = count_level;                                                                      //改最大层数
                Tree_count[count_level] = 0;                                                                 //开始计数
            }
            root_point->point_left_child->order = root_point->order * 2;                              //右节点的位数等于母结点位数*2
            root_point->point_left_child->level = count_level;                                        //写层数
            Tree_count[count_level]++;                                                                //这一层多一个元素
            Tree[count_level][(int)(count * pow(2, count_level)) - 1] = root_point->point_left_child; // Tree指向此元素
            count_level = 0;                                                                          //恢复全局变量
            printf("创建完成\r\n");
            return;
        }

        creat_point(root_point->point_left_child, key, count);
    }
    else if (key == root_point->data->key)
    {
        count_level = 0;
        printf("已经添加该元素");
        return;
    }
}
//查找结点
Tree_point *search_tree_point(Tree_point *root_point, int key)
{
    if (!root_point) //如果没有根节点
        return NULL;
    if (key == root_point->data->key) //根节点就是这个点
        return root_point;
    else if (key > root_point->data->key)
        return search_tree_point(root_point->point_right_child, key);
    else if (key < root_point->data->key)
        return search_tree_point(root_point->point_left_child, key);
}
//删除结点
void Delete(Tree_point *root_point, int key)
{
    Tree_point *p = root_point;
    Tree_point *f = NULL;
    while (p)
    {
        if (p->data->key == key)
            break;
        if (p->data->key > key)
        {
            p = p->point_left_child;
        }
        else
        {
            p = p->point_right_child;
        }
    }
    if (!p)
    {
        printf("没找到该元素");
        return;
    }
    //获取到了要删除的结点p
    Tree_point *q = next_point(root_point, p);                       // q是p的中序遍历下一个结点
    if (p->point_right_child != NULL && p->point_left_child != NULL) //如果p左右节点都存在
    {                                                                // pq交换位置后，p仍是需要删除的结点，且p不再左右都有结点
        int key = p->data->key;
        p->data->key = q->data->key;
        q->data->key = key;
        Tree_point *c;
        c = p;
        p = q;
        q = c;
    }
    if (p->point_right_child == NULL && p->point_left_child == NULL) //如果左右节点都不存在
    {
        if (p->point_father) //如果不是头结点
        {                    //父节点指空
            (p->point_father->point_left_child == p) ? (p->point_father->point_left_child = NULL) : (p->point_father->point_right_child = NULL);
        }
        Tree[p->level][p->order] = &example; // Tree里恢复初始化
        Tree_count[p->level] -= 1;           //本层元素-1
        if (Tree_count[p->level] == 0)       //如果这是本层最后一个元素
        {                                    //最大层数-1
            maxlevel -= 1;
        }
        free(p);
    }
    else if (p->point_right_child == NULL && p->point_left_child != NULL) //如果只有左结点
    {
        if (p->point_father) //如果不是头结点
        {                    //父节点指向左节点
            (p->point_father->point_left_child == p) ? (p->point_father->point_left_child = p->point_left_child) : (p->point_father->point_right_child = p->point_left_child);
        }
        Tree[p->level][p->order] = p->point_left_child;                          // Tree指向左节点
        Tree[p->point_left_child->level][p->point_left_child->order] = &example; //原本左节点Tree的位置恢复初始化
        Tree_count[p->level + 1] -= 1;                                           //原本左节点所在层元素数-1
        free(p);
    }
    else if (p->point_right_child != NULL && p->point_left_child == NULL) //如果只有右节点
    {
        if (p->point_father) //如果不是头结点
        {                    //父节点指向右结点
            (p->point_father->point_left_child == p) ? (p->point_father->point_left_child = p->point_right_child) : (p->point_father->point_right_child = p->point_right_child);
        }
        Tree[p->level][p->order] = p->point_right_child;                           // Tree指向右节点
        Tree[p->point_right_child->level][p->point_right_child->order] = &example; //原本右节点Tree的位置恢复初始化
        Tree[p->level + 1] -= 1;                                                   //原本右节点所在层元素数-1
        free(p);
    }
}
//打印二叉树
void print_Tree(Tree_point *root_point)
{
    int i, j, k;
    int depth = maxlevel + 1;
    for (j = 0; j < depth; j++)
    {
        int w = pow(2, depth - j + 1);
        if (j == 0)
            printf("%*c\n", w, ' ');
        else
        {
            for (i = 0; i < pow(2, j - 1); i++)
            {
                printf("%*c", w + 1, ' ');
                for (k = 0; k < w - 3; k++)
                    printf("_"); //左横
                printf("/ \\");
                for (k = 0; k < w - 3; k++)
                    printf("_"); //右横
                printf("%*c", w + 2, ' ');
            }
            printf("\n");
            for (i = 0; i < pow(2, j - 1); i++)
                printf("%*c/%*c %*c", w, ' ', w * 2 - 2, '\\', w, ' ');
            printf("\n");
        }
        for (i = 0; i < pow(2, j); i++) //写数据部分
        {
            if (Tree[j][i] != &example)
            {
                if (Tree[j][i] < 0)
                    Tree[j][i]->data->key = -1;
                int h = adjust(Tree[j][i]->data->key);
                printf("%*c%d)%*c", w - 1, '(', Tree[j][i]->data->key, w - h, ' ');
            }
            else
                printf("%*c_)%*c", w - 1, '(', w - 1, ' '); //如果没有元素就写_
        }
        printf("\n");
    }
}
//判断位数
int adjust(int num)
{
    int count = 0;
    while (num != 0)
    {
        // n = n/10
        num /= 10;
        ++count;
    }
    return count;
}

//中序遍历
void inorder_traversal(Tree_point *root_point)
{
    if (root_point->point_left_child == NULL && root_point->point_right_child == NULL) //如果左右子树都为空
        printf("%d ", root_point->data->key);
    else if (root_point->point_left_child == NULL) //只有右结点
    {
        printf("%d ", root_point->data->key);
        inorder_traversal(root_point->point_right_child);
    }
    else if (root_point->point_right_child == NULL) //只有左结点
    {
        inorder_traversal(root_point->point_left_child);
        printf("%d ", root_point->data->key);
    }
    else //左右结点都存在
    {
        inorder_traversal(root_point->point_left_child);
        printf("%d ", root_point->data->key);
        inorder_traversal(root_point->point_right_child);
    }
}

//找到某点中序遍历中的下一个点，找到后递归返回
Tree_point *next_point(Tree_point *root_point, Tree_point *some_point)
{
    if (root_point->point_left_child == NULL && root_point->point_right_child == NULL) //如果左右子树都为空
    {
        if (next_point_flag == 0)
        {
            next_point_flag = 1;
            return root_point;
        }
        if (root_point == some_point)
        {
            next_point_flag = 0;
        }
    }
    else if (root_point->point_left_child == NULL)
    {
        Tree_point *a = next_point(root_point->point_right_child, some_point);
        if (a)
            return a;
    }
    else if (root_point->point_right_child == NULL)
    {
        Tree_point *b = next_point(root_point->point_left_child, some_point);
        if (b)
            return b;
    }
    else
    {
        Tree_point *c = next_point(root_point->point_left_child, some_point);
        if (c)
            return c;
        if (next_point_flag == 0)
        {
            next_point_flag = 1;
            return root_point;
        }
        if (root_point == some_point)
        {
            next_point_flag = 0;
        }
        Tree_point *d = next_point(root_point->point_right_child, some_point);
        if (d)
            return d;
    }
    return NULL;
}
