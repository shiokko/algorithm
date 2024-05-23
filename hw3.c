#include<stdio.h>
#include<stdbool.h>

bool isCyclicUtil(int v, bool visited[], int parent, int n, int graph[n][n]) {
    visited[v] = true;

    for (int i = 0; i < n; i++) {
        if (graph[v][i]) {
            if (!visited[i]) {
                if (isCyclicUtil(i, visited, v, n, graph))
                    return true;
            } else if (i != parent) {
                return true;
            }
        }
    }
    return false;
}

bool hasCycle(int n, int graph[n][n]) {
    bool visited[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            if (isCyclicUtil(i, visited, -1, n, graph)) {
                return 1;
            }
        }
    }
    return 0;
}

void DFS(int v, bool visited[], int n, int graph[n][n]) {
    visited[v] = true;
    for (int i = 0; i < n; i++) {
        if (graph[v][i] && !visited[i]) {
            DFS(i, visited, n, graph);
        }
    }
}

int area(int n, int graph[n][n]) {
    bool visited[n];
    for (int i = 0; i < n; i++) {
        visited[i] = false;
    }

    int count = 0;
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            DFS(i, visited, n, graph);
            count++;
        }
    }
    return count;
}

int main(){
    int n;
    scanf("%d", &n);
    int graph[n][n];
    for(int i = 0; i < n ; i++){
        for(int j = 0 ;j < n; j++)
            graph[i][j] = 0;
    }
    for (int i = 0; i < n; i++) {
        int k;
        scanf("%d", &k);
        int j;
        while (scanf("%d", &j) != EOF) {
            // printf("%d", j);
            graph[k-1][j-1] = 1;
            graph[j-1][k-1] = 1;
            // edgeCount++;
            if (getchar() == '\n') {
                break;
            }
        }
    }

    int P = area(n, graph) -1;
    int Q = hasCycle(n, graph);

    printf("%d %d", P, Q);
}