#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>

#define intmax 2147483647

int minint(int a, int b){
    if(a < b)
        return a;
    else
        return b;
    return 0;
}

int isUpperCaseLetter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}


typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct tt{
    int time;
    char name[3];
} tt;

// 創建新節點
Node* createNode(int data, Node* next) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = next;
    return newNode;
}

typedef struct line{
    char name[3];
    int awaytime;
    bool visited;
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
    newline->awaytime = intmax;
    newline->visited = false;
    return newline;
}

int solve1(int x, int y,line* lines[1001], Node* station){
    int t;
    Node* nodeptr;
    int minarrive[1001];
    for(int i = 0; i < 1001; i++)
        minarrive[i] = intmax;
    line* lineptr1;
    line* lineptr2;
    nodeptr = station;
    while(nodeptr != NULL){
        lineptr1 = lines[nodeptr->data];
        if(lineptr1!=NULL){
            lineptr1->visited = false;
            lineptr1 = lineptr1->next;
        }
        nodeptr = nodeptr->next;
    }
    lineptr1 = lines[x];
    minarrive[x] = 0;
    while(lineptr1 != NULL){
        lineptr2 = lines[lineptr1->target];
        minarrive[lineptr1->target] = lineptr1->time;
        while(lineptr2 != NULL){
            if(lineptr1->name[1] == lineptr2->name[1] && lineptr1->name[0] == lineptr2->name[0])
                t = lineptr1->time;
            else if(isUpperCaseLetter(lineptr2->name[0]))
                t = lineptr1->time + 10;
            else
                t = lineptr1->time + 5;
            lineptr2->awaytime = t;
            lineptr2 = lineptr2->next;
        }
        lineptr1->visited = true;
        lineptr1 = lineptr1->next;
    }
    while(true){
        int min = intmax;
        line* target;
        nodeptr = station;
        while(nodeptr != NULL){
            lineptr1 = lines[nodeptr->data];
            while(lineptr1 != NULL){
                if(!lineptr1->visited && lineptr1->awaytime < intmax){
                    target = lineptr1;
                    min = lineptr1->awaytime;
                }
                lineptr1 = lineptr1->next;    
            }
            nodeptr = nodeptr->next;
        }
        if(min == intmax)
            break;
        lineptr1 = target;
        lineptr1->visited = true;
        minarrive[lineptr1->target] = minint(minarrive[lineptr1->target],lineptr1->awaytime+lineptr1->time);
        lineptr2 = lines[lineptr1->target];
        while(lineptr2 != NULL){
            if(lineptr1->name[1] == lineptr2->name[1] && lineptr1->name[0] == lineptr2->name[0])
                t = lineptr1->awaytime + lineptr1->time;
            else if(isUpperCaseLetter(lineptr2->name[0]))
                t = lineptr1->awaytime + lineptr1->time + 10;
            else
                t = lineptr1->awaytime + lineptr1->time + 5;
            lineptr2->awaytime = minint(lineptr2->awaytime, t);
            lineptr2 = lineptr2->next;
        }
    }
    if(minarrive[y]== intmax)
        return -1;
    else
        return minarrive[y];
}

void next_sta(int target, char name[3],tt time[1001], line* lines[1001],int t){
    line* lineptr = lines[target];
    while(lineptr!=NULL){
        if(lineptr->name[0] == name[0] && lineptr->name[1] == name[1]){
            t = t+lineptr->time;
            if(time[lineptr->target].time > t){
               time[lineptr->target].time = t;
               strcpy(time[lineptr->target].name, name);
            }
            if(t <= (time[lineptr->target].time+10))
                next_sta(lineptr->target, name, time, lines, t);
            break;
        }
        lineptr = lineptr->next;
    }
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
                next_sta(lineptr->target, lineptr->name, time, lines, t);
                if(t < time[lineptr->target].time){
                    time[lineptr->target].time = t;
                    strcpy(time[lineptr->target].name, lineptr->name);
                }
                lineptr = lineptr->next;
            }
        }
    if(time[y].time == intmax)
        return -1;
    else
        return time[y].time;
}

int main(){
    int count = 0;
    Node* station = NULL;
    line* lines[1001];
    line* lineptr;
    Node* nodeptr;
    for(int i = 0; i < 1001; i++)
        lines[i] = NULL;
    char name[3];
    char commend;
    int s, x, y, t;
    bool hasstation[1001];
    tt time[1001];
    for(int i = 0 ;i < 1001; i++)
        hasstation[i] = false;
    while (scanf(" %c", &commend) != EOF) {
        if(commend == 'L'){
            scanf(" %s %d", name, &s);
            for (int i = 0; i < s; i++) {
                scanf("%d %d %d", &x, 
                                &y, 
                                &t);
                count++;
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
            if(count < 500)
                printf("%d\n", solve1(x, y, lines, station));
            else
                printf("%d\n", solve(x, y, time, lines, station));

        }else
            break;
    }
}