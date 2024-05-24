#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX_STATIONS 1001
#define MAX_EDGES 2000

typedef struct {
    int to;
    int time;
    int type; // 0: bus, 1: metro
    char line_name[10];
} Edge;

typedef struct {
    int station;
    int time;
    int type; // 0: bus, 1: metro
    char line_name[10];
} Node;

typedef struct {
    Node nodes[MAX_EDGES];
    int size;
} PriorityQueue;

void swap(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void pq_push(PriorityQueue* pq, Node node) {
    pq->nodes[pq->size] = node;
    int i = pq->size++;
    while (i && pq->nodes[(i - 1) / 2].time > pq->nodes[i].time) {
        swap(&pq->nodes[i], &pq->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Node pq_pop(PriorityQueue* pq) {
    Node top = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    int i = 0;
    while ((2 * i + 1) < pq->size) {
        int smallest = (2 * i + 1);
        if ((2 * i + 2) < pq->size && pq->nodes[2 * i + 2].time < pq->nodes[smallest].time)
            smallest = (2 * i + 2);
        if (pq->nodes[i].time <= pq->nodes[smallest].time)
            break;
        swap(&pq->nodes[i], &pq->nodes[smallest]);
        i = smallest;
    }
    return top;
}

Edge graph[MAX_STATIONS][MAX_STATIONS][2];
int dist[MAX_STATIONS][2];
int visited[MAX_STATIONS][2];

void dijkstra(int start, int n) {
    PriorityQueue pq = { .size = 0 };
    for (int i = 1; i <= n; i++) {
        dist[i][0] = dist[i][1] = INT_MAX;
        visited[i][0] = visited[i][1] = 0;
    }
    dist[start][0] = dist[start][1] = 0;
    pq_push(&pq, (Node){ .station = start, .time = 0, .type = 0, .line_name = "" });
    pq_push(&pq, (Node){ .station = start, .time = 0, .type = 1, .line_name = "" });

    while (pq.size) {
        Node curr = pq_pop(&pq);
        int u = curr.station, t = curr.type;
        if (visited[u][t]) continue;
        visited[u][t] = 1;

        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < 2; j++) {
                if (graph[u][i][j].to != -1 && !visited[i][j]) {
                    int transfer_time = 0;
                    if (strcmp(curr.line_name, graph[u][i][j].line_name) != 0) {
                        if (t == 0 && j == 1) transfer_time = 10; // bus to metro
                        if (t == 1 && j == 0) transfer_time = 5;  // metro to bus
                        if (t == 0 && j == 0) transfer_time = 5;  // bus to bus
                        if (t == 1 && j == 1) transfer_time = 10; // metro to metro
                    }

                    int new_time = dist[u][t] + graph[u][i][j].time + (u != start ? transfer_time : 0);
                    if (new_time < dist[i][j]) {
                        dist[i][j] = new_time;
                        Node next_node = { .station = i, .time = new_time, .type = j };
                        strcpy(next_node.line_name, graph[u][i][j].line_name);
                        pq_push(&pq, next_node);
                    }
                }
            }
        }
    }
}

int main() {
    int n = MAX_STATIONS;
    for (int i = 0; i < MAX_STATIONS; i++)
        for (int j = 0; j < MAX_STATIONS; j++)
            graph[i][j][0] = graph[i][j][1] = (Edge){ .to = -1, .time = 0, .type = -1, .line_name = "" };

    char line[256];
    while (fgets(line, sizeof(line), stdin)) {
        if (line[0] == 'L') {
            char type[10];
            int s;
            sscanf(line, "L %s %d", type, &s);
            int route_type = (type[0] >= 'A' && type[0] <= 'Z') ? 1 : 0;
            for (int i = 0; i < s; i++) {
                int x, y, t;
                fgets(line, sizeof(line), stdin);
                sscanf(line, "%d %d %d", &x, &y, &t);
                graph[x][y][route_type] = (Edge){ .to = y, .time = t, .type = route_type };
                strcpy(graph[x][y][route_type].line_name, type);
            }
        } else if (line[0] == 'Q') {
            int start, end;
            sscanf(line, "Q %d %d", &start, &end);
            dijkstra(start, n);
            int min_time = (dist[end][0] < dist[end][1]) ? dist[end][0] : dist[end][1];
            if (min_time == INT_MAX) min_time = -1;
            printf("%d\n", min_time);
        } else if (line[0] == 'E') {
            break;
        }
    }
    return 0;
}