#include<stdio.h>

typedef struct wood{
    int start;
    int end;
}wood; 

void swap(struct wood *a, struct wood *b) {
    struct wood temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(struct wood arr[], int n) {
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j].start == arr[min_idx].start) {
                if (arr[j].end < arr[min_idx].end) {
                    min_idx = j;
                }
            }
            else if (arr[j].start < arr[min_idx].start) {
                min_idx = j;
            }
        }
        swap(&arr[min_idx], &arr[i]);
    }
}

int main(){
    int n;
    scanf("%d", &n);
    wood woods[n];

    for(int i = 0; i < n; i++){
        scanf("%d %d", &woods[i].start, &woods[i].end);
    }

    int goal = woods[n-1].start;
    int counter = 1;
    int end_pos = woods[0].end;
    int max_end = 0;
    int used = 0;
    int next;

    selectionSort(woods, n);
    
    // for(int i = 0; i < n; i++){
    //     printf("%d %d\n", woods[i].start, woods[i].end);
    // }

    while(end_pos <= goal){
        while(woods[used].start < end_pos){
            if(woods[used].end > max_end){
                next = used;
            }
            used++;
        }
        counter++;
        end_pos = woods[next].end;
    }

    printf("%d", counter);
}