#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>

#define intmax 2147483647

int isUpperCaseLetter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

typedef struct tt{
    int time;
    char name[3];
} tt;

typedef struct Node {
    int data;
    struct Node* next;
} Node;

// 創建新節點
Node* createNode(int data, Node* next) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = next;
    return newNode;
}

typedef struct line{
    char name[3];
    int target;
    int time;
    struct line* next;
}line; 

line* createline(char name[3], int target, int time, line* next) {
    line* newline = (line*)malloc(sizeof(line));
    strcpy(newline->name, name);
    newline->target = target;
    newline->time = time;
    newline->next = next;
    return newline;
}

int solve(int x, int y, tt time[1001],line* lines[1001], Node* station){
    int t;
    bool visited[1001];
    Node* nodeptr;
    line* lineptr;
    for(int i = 0; i < 1001; i++){
            time[i].time = intmax;
            visited[i] = false;
        }
        time[x].time = 0;
        lineptr = lines[x];
        visited[x] = true;
        while(lineptr != NULL){
            time[lineptr->target].time = lineptr->time;
            strcpy(time[lineptr->target].name, lineptr->name);
            lineptr = lineptr->next;
        }
        // nodeptr = station;
        // while(nodeptr != NULL){
        //     printf("%d %d ", nodeptr->data, time[nodeptr->data]);
        //     nodeptr = nodeptr->next;
        // }
        while(true){
            int min = intmax;
            int target;
            nodeptr = station;
            while(nodeptr != NULL){
                if(!visited[nodeptr->data] && time[nodeptr->data].time < intmax){
                    target = nodeptr->data;
                    min = time[nodeptr->data].time;
                }
                nodeptr = nodeptr->next;
            }
            if(min == intmax)
                break;
            lineptr = lines[target];
            visited[target] = true;
            while(lineptr != NULL){
                if(lineptr->name[1] == time[target].name[1] && lineptr->name[0] == time[target].name[0])
                    t = time[target].time + lineptr->time;
                else if(isUpperCaseLetter(lineptr->name[0]))
                    t = time[target].time + lineptr->time + 10;
                else
                    t = time[target].time + lineptr->time + 5;
                if(t < time[lineptr->target].time){
                    time[lineptr->target].time = t;
                    strcpy(time[lineptr->target].name, lineptr->name);
                }
                lineptr = lineptr->next;
            }
            // nodeptr = station;
            // while(nodeptr != NULL){
            //     printf("%d %d ", nodeptr->data, time[nodeptr->data]);
            //     nodeptr = nodeptr->next;
            // }
            // printf("\n");
        }
    if(time[y].time == intmax)
        return -1;
    else
        return time[y].time;
}

int main(){
    Node* station = NULL;
    line* lines[1001];
    line* lineptr;
    Node* nodeptr;
    for(int i = 0; i < 1001; i++)
        lines[i] = NULL;
    char name[3];
    char commend;
    int s, x, y, t;
    tt time[1001];
    bool hasstation[1001];

    for(int i = 0 ;i < 1001; i++)
        hasstation[i] = false;
    while (scanf(" %c", &commend) != EOF) {
        if(commend == 'L'){
            scanf(" %s %d", name, &s);
            for (int i = 0; i < s; i++) {
                scanf("%d %d %d", &x, 
                                &y, 
                                &t);
                lines[x] = createline(name, y, t, lines[x]);
                if(!hasstation[x]){
                    station = createNode(x, station);
                    hasstation[x] = true;
                }
                if(!hasstation[y]){
                    station = createNode(y, station);
                    hasstation[y] = true;
                }
            }
        }else if(commend == 'Q'){
            scanf(" %d %d", &x, &y);
            printf("%d\n", solve(x, y, time, lines, station));
        }else
            break;
    }
}