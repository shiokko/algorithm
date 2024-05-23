#include<stdio.h>
#include <stdlib.h>
void insertionSort(int arr[][2], int rows) {
    int i, j;
    int key[2];

    for (i = 1; i < rows; i++) {
        key[0] = arr[i][0];
        key[1] = arr[i][1];
        j = i - 1;

        while (j >= 0 && arr[j][1] > key[1]) {
            arr[j + 1][0] = arr[j][0];
            arr[j + 1][1] = arr[j][1];
            j = j - 1;
        }

        arr[j + 1][0] = key[0];
        arr[j + 1][1] = key[1];
    }
}

int max(int a, int b){
    if(a > b)
        return a;
    return b;
}

int main(){
    int dp[3651];
    int n;
    scanf("%d", &n);
    int job[n][2];
    for(int i = 0; i < n; i++)
        scanf("%d %d", &job[i][0], &job[i][1]);
    insertionSort(job, n);
    int start, end;
    scanf("%d %d", &start, &end);
    int counter = 0;
    while(1){
        if(job[counter][1] < start)
            counter++;
        else
            break;
    }
    dp[start-1] = 0;
    for(int i = start; i <= end; i++){
        dp[i] = dp[i-1];
        while(1){
            if(counter>=n)
                break;
            if(job[counter][1] == i){
                if(job[counter][0]>=start){
                    dp[i] = max(dp[i],dp[job[counter][0]]+job[counter][1]-job[counter][0]);
                }
                counter++;
            }else
                break;
        }
    }
    printf("%d", dp[end]);
}