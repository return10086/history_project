#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include <string.h>
#define MAX_VERTICES 100
#define INF 9999
#define MAX_NAME_LENGTH 100
// ��Ľṹ��
typedef struct
{
    wchar_t name[50]; // ��������
    int index;        // ������ͼ�е�����
} Place;

// ͼ�Ľṹ��
typedef struct
{
    int weight[MAX_VERTICES][MAX_VERTICES];      // �ߵ�Ȩֵ
    wchar_t name[MAX_VERTICES][MAX_NAME_LENGTH]; // �ص���������
    Place places[MAX_VERTICES];                  // �����б�
    int numVertices;                             // �������
    int numEdges;                                // �ߵ�����
} Graph;

// ��ʼ��ͼ
void initGraph(Graph *graph)
{
    memset(graph->weight, INF, sizeof(graph->weight));
    graph->numVertices = 0;
    graph->numEdges = 0;
}

// ���ӳ�����ͼ��
void addPlace(Graph *graph, const wchar_t *name, int index)
{
    wcscpy(graph->places[index].name, name);
    graph->places[index].index = index;
    graph->numVertices++;
}

// ��������ߵ�ͼ��
void addEdge(Graph *graph, int src, int dest, int weight, const wchar_t *pathName)
{
    graph->weight[src][dest] = weight;
    graph->weight[dest][src] = weight; // �����˫��ߣ�ҲҪ���������Ȩ��

    wcscpy(graph->name[src], pathName);
    wcscpy(graph->name[dest], pathName); // �����˫��ߣ�ҲҪ��������ĵص�����
}

// ��ȡ������ͼ�е�����
int getPlaceIndex(Graph *graph, const wchar_t *name)
{
    int i;
    for (i = 0; i < graph->numVertices; i++)
    {
        if (wcscmp(graph->places[i].name, name) == 0)
            return i;
    }
    return -1;
}
// Dijkstra�㷨�ҳ����·��
void findShortestPath(const Graph *graph, int start, int end)
{
    int dist[MAX_VERTICES];     // �洢����㵽ÿ���ڵ����̾���
    int previous[MAX_VERTICES]; // �洢ÿ���ڵ��ǰ���ڵ㣬���ڹ������·��
    bool visited[MAX_VERTICES]; // ��ǽڵ��Ƿ񱻷��ʹ�

    for (int i = 0; i < graph->numVertices; i++)
    {
        dist[i] = INF;      // ��ʼ������Ϊ�����
        previous[i] = -1;   // ��ʼ��ǰ���ڵ�Ϊ-1
        visited[i] = false; // ��ʼ���ڵ�δ���ʹ�
    }

    dist[start] = 0; // ��㵽�����ľ���Ϊ0

    // ʹ��Dijkstra�㷨�������·��
    for (int count = 0; count < graph->numVertices - 1; count++)
    {
        int minDist = INF;
        int u = -1;

        // ѡ�������������δ���ʽڵ���Ϊ��һ���м�ڵ�
        for (int v = 0; v < graph->numVertices; v++)
        {
            if (!visited[v] && dist[v] < minDist)
            {
                minDist = dist[v];
                u = v;
            }
        }

        visited[u] = true; // ��ǽڵ�uΪ�ѷ���

        // ��������δ���ʽڵ����̾����ǰ���ڵ�
        for (int v = 0; v < graph->numVertices; v++)
        {
            if (!visited[v] && graph->weight[u][v] != INF)
            {
                int newDist = dist[u] + graph->weight[u][v];
                if (newDist < dist[v])
                {
                    dist[v] = newDist;
                    previous[v] = u;
                }
            }
        }
    }

    // ������·��
    wprintf(L"��%s��%s�����·��Ϊ��\n", graph->places[start].name, graph->places[end].name);

    int currentNode = end;
    int path[MAX_VERTICES];
    int pathLength = 0;

    // ����ǰ���ڵ����鹹�����·��
    while (currentNode != start)
    {
        path[pathLength++] = currentNode;
        currentNode = previous[currentNode];
    }

    path[pathLength++] = start; // ��������ӵ�·��ĩβ

    // ���·��˳��
    for (int i = pathLength - 1; i >= 0; i--)
    {
        wprintf(L"%ls", graph->places[path[i]].name);
        if (i > 0)
        {
            wprintf(" �� ");
        }
    }
    wprintf(L"\n");

    // ���·��Ȩֵ�ܺ�
    wprintf("·���ܳ���Ϊ��%d ��\n ", dist[end]);
}

int main()
{
    setlocale(LC_ALL, "");

    Graph graph;
    int i;

    initGraph(&graph);

    // ��ͼ�����ӳ���
    addPlace(&graph, "����", 0);
    addPlace(&graph, "���ɷ�·���Ⱥ�Ȫ··��", 1);
    addPlace(&graph, "����", 2);
    addPlace(&graph, "���±�·����ʨ��··��", 3);
    addPlace(&graph, L"���±�·�Ͳ�����··��", 4);
    addPlace(&graph, L"�����ٳ�", 5);
    addPlace(&graph, L"УҽԺ", 6);
    addPlace(&graph, L"һ������", 7);
    addPlace(&graph, L"��������", 8);
    addPlace(&graph, L"��������", 9);
    addPlace(&graph, L"ͼ���", 10);
    addPlace(&graph, L"����¥", 11);
    addPlace(&graph, L"����¥", 12);
    addPlace(&graph, L"��Ϣ¥", 13);
    addPlace(&graph, L"�Ϲ���", 14);
    addPlace(&graph, L"����¥", 15);
    addPlace(&graph, L"������", 16);
    addPlace(&graph, L"����ٳ�", 17);
    addPlace(&graph, L"��������", 18);
    addPlace(&graph, L"ʱ��㳡", 19);
    addPlace(&graph, L"���Ǻ�", 20);
    addPlace(&graph, L"������·���Ⱥ�Ȫ··��", 21);
    addPlace(&graph, L"������·�ʹ���··��", 22);
    addPlace(&graph, L"����·�����ɷ�··��", 23);
    addPlace(&graph, L"�Ⱥ�Ȫ·��������··��", 24);
    addPlace(&graph, L"���Ȫ·����ʨ��··��", 25);
    addPlace(&graph, L"���Ȫ·�ͺ�ˮ��··��", 26);
    addPlace(&graph, L"������·�ͺ�ˮ��··��", 27);
    addPlace(&graph, L"�Ⱥ�Ȫ·��������··��", 28);
    addPlace(&graph, L"����ֱ��·��һ��������ͨ··��", 29);
    addPlace(&graph, L"һ��������ͨ··�ʹ���··��", 30);
    addPlace(&graph, L"һ����������·��������··��", 31);
    addPlace(&graph, L"һ����������·��һ��������ͨ··��", 32);
    addPlace(&graph, L"������ת·�������ٳ���··��", 33);
    addPlace(&graph, L"�����ٳ���·��һ��������ͨ··��", 34);
    addPlace(&graph, L"������·��ͭȸ̨··��", 35);
    addPlace(&graph, L"ͭȸ̨·�����Ŵ�··��", 36);
    addPlace(&graph, L"ͭȸ̨·����ʨ��··��", 37);
    addPlace(&graph, L"�����ٳ���·��һ��������ͨ··��", 38);
    addPlace(&graph, L"������·��ͭȸ̨··��", 39);
    addPlace(&graph, L"ͭȸ̨·�����Ŵ�··��", 40);
    addPlace(&graph, L"���Ȫ·�����ɷ�··��", 41);
    addPlace(&graph, L"������·����ʨ��··��", 42);
    addPlace(&graph, L"��ˮ��·ʮ��·��", 43);
    addPlace(&graph, L"���Ȫ··��", 44);

    // �ص���������
    wcscpy(graph.places[0].name, L"����");
    wcscpy(graph.places[1].name, L"���ɷ�·���Ⱥ�Ȫ··��");
    wcscpy(graph.places[2].name, L"����");
    wcscpy(graph.places[3].name, L"���±�·����ʨ��··��");
    wcscpy(graph.places[4].name, L"���±�·�Ͳ�����··��");
    wcscpy(graph.places[5].name, L"�����ٳ�");
    wcscpy(graph.places[6].name, L"УҽԺ");
    wcscpy(graph.places[7].name, L"һ������");
    wcscpy(graph.places[8].name, L"��������");
    wcscpy(graph.places[9].name, L"��������");
    wcscpy(graph.places[10].name, L"ͼ���");
    wcscpy(graph.places[11].name, L"����¥");
    wcscpy(graph.places[12].name, L"����¥");
    wcscpy(graph.places[13].name, L"��Ϣ¥");
    wcscpy(graph.places[14].name, L"�Ϲ���");
    wcscpy(graph.places[15].name, L"����¥");
    wcscpy(graph.places[17].name, L"����ٳ�");
    wcscpy(graph.places[18].name, L"��������");
    wcscpy(graph.places[19].name, L"ʱ��㳡");
    wcscpy(graph.places[20].name, L"���Ǻ�");
    wcscpy(graph.places[21].name, L"������·���Ⱥ�Ȫ··��");
    wcscpy(graph.places[22].name, L"������·�ʹ���··��");
    wcscpy(graph.places[23].name, L"����·�����ɷ�··��");
    wcscpy(graph.places[24].name, L"�Ⱥ�Ȫ·��������··��");
    wcscpy(graph.places[25].name, L"���Ȫ·����ʨ��··��");
    wcscpy(graph.places[26].name, L"���Ȫ·�ͺ�ˮ��··��");
    wcscpy(graph.places[27].name, L"������·�ͺ�ˮ��··��");
    wcscpy(graph.places[28].name, L"�Ⱥ�Ȫ·��������··��");
    wcscpy(graph.places[29].name, L"����ֱ��·��һ��������ͨ··��");
    wcscpy(graph.places[30].name, L"һ��������ͨ··�ʹ���··��");
    wcscpy(graph.places[31].name, L"һ����������·��������··��");
    wcscpy(graph.places[32].name, L"һ����������·��һ��������ͨ··��");
    wcscpy(graph.places[33].name, L"������ת·�������ٳ���··��");
    wcscpy(graph.places[34].name, L"�����ٳ���·��һ��������ͨ··��");
    wcscpy(graph.places[35].name, L"������·��ͭȸ̨··��");
    wcscpy(graph.places[36].name, L"ͭȸ̨·�����Ŵ�··��");
    wcscpy(graph.places[37].name, L"ͭȸ̨·����ʨ��··��");
    wcscpy(graph.places[38].name, L"�����ٳ���·��һ��������ͨ··��");
    wcscpy(graph.places[39].name, L"������·��ͭȸ̨··��");
    wcscpy(graph.places[40].name, L"ͭȸ̨·�����Ŵ�··��");
    wcscpy(graph.places[41].name, L"���Ȫ·�����ɷ�··��");
    wcscpy(graph.places[42].name, L"������·����ʨ��··��");
    wcscpy(graph.places[43].name, L"��ˮ��·ʮ��·��");
    wcscpy(graph.places[44].name, L"���Ȫ··��");

    // ����·����Ȩֵ
    addEdge(&graph, 2, 4, 240, L"���±�·"); // ���Ϊ���յ�Ϊ��·������,·������
    addEdge(&graph, 4, 3, 120, L"���±�·");
    addEdge(&graph, 4, 21, 380, L"������·");
    addEdge(&graph, 21, 18, 70, L"�Ⱥ�Ȫ·");
    addEdge(&graph, 21, 17, 100, L"�Ⱥ�Ȫ·");
    addEdge(&graph, 17, 1, 100, L"�Ⱥ�Ȫ·");
    addEdge(&graph, 1, 41, 90, L"���ɷ�·");
    addEdge(&graph, 12, 1, 190, L"���ɷ�·");
    addEdge(&graph, 12, 11, 100, L"����·");
    addEdge(&graph, 11, 6, 440, L"����·");
    addEdge(&graph, 24, 1, 362, L"�Ⱥ�Ȫ·");
    addEdge(&graph, 15, 41, 10, L"���ɷ�·");
    addEdge(&graph, 24, 27, 260, L"������·");
    addEdge(&graph, 27, 31, 62, L"������·");
    addEdge(&graph, 31, 35, 165, L"������");
    addEdge(&graph, 35, 40, 270, L"ͭȸ̨·");
    addEdge(&graph, 40, 13, 270, L"ͭȸ̨·");
    addEdge(&graph, 13, 42, 104, L"��ʨ��·");
    addEdge(&graph, 2, 42, 360, L"������·");
    addEdge(&graph, 42, 3, 50, L"��ʨ��·");
    addEdge(&graph, 38, 6, 237, L"һ��������ͨ·");
    addEdge(&graph, 38, 5, 94, L"�����ٳ���·");
    addEdge(&graph, 5, 33, 87, L"�����ٳ���·");
    addEdge(&graph, 33, 0, 390, L"������ת·");
    addEdge(&graph, 0, 9, 200, L"������ת·");
    addEdge(&graph, 9, 29, 110, L"һ��������ͨ·");
    addEdge(&graph, 29, 7, 57, L"һ��������ͨ·");
    addEdge(&graph, 7, 8, 10, L"�ᴩһ��������ͨ·");
    addEdge(&graph, 12, 28, 160, L"�Ⱥ�Ȫ·");
    addEdge(&graph, 10, 19, 50, L"�ᴩ�㳡");
    addEdge(&graph, 10, 43, 50, L"��ͼ�����ת");
    addEdge(&graph, 10, 36, 50, L"��ͼ�����ת");
    addEdge(&graph, 15, 43, 30, L"���Ȫ·");
    addEdge(&graph, 43, 44, 168, L"���Ȫ·");
    addEdge(&graph, 41, 44, 147, L"���Ȫ·");
    addEdge(&graph, 14, 36, 190, L"���Ŵ�·");
    addEdge(&graph, 20, 15, 153, L"�ᴩ��԰�ʹ��Ȫ·");
    addEdge(&graph, 20, 8, 50, L"�ᴩ��԰��������·");
    addEdge(&graph, 19, 3, 40, L"�߻���");
    addEdge(&graph, 19, 42, 40, L"�ᴩ�㳡");
    addEdge(&graph, 19, 43, 30, L"�ߺ�ˮ��·");
    addEdge(&graph, 19, 15, 30, L"������");
    addEdge(&graph, 0, 29, 92, L"����ֱ��·");

    // �ҳ����·��
    wchar_t start[45], end[45];
    wcsncpy(start, L"start", sizeof(start) / sizeof(wchar_t) - 1); // �����ַ���
    start[sizeof(start) / sizeof(wchar_t) - 1] = L'\0';            // ��������ֹ��
    wcsncpy(end, L"end", sizeof(end) / sizeof(wchar_t) - 1);
    end[sizeof(end) / sizeof(wchar_t) - 1] = L'\0';

    printf("����������غ�Ŀ�ĵص����ƣ�\n");
    wscanf(L"%ls %ls", start, end);

    int startIdx = -1, endIdx = -1;

    for (i = 0; i < graph.numVertices; i++)
    {
        if (wcscmp(graph.places[i].name, start) == 0)
        {
            startIdx = i;
        }
        if (wcscmp(graph.places[i].name, end) == 0)
        {
            endIdx = i;
        }
    }

    if (startIdx == -1 || endIdx == -1)
    {
        printf("�����ػ�Ŀ�ĵز�����\n");
        return 0;
    }

    findShortestPath(&graph, startIdx, endIdx);

    return 0;
}
