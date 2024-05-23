#include<stdio.h>

int max(int a, int b){
    if(a > b)
        return a;
    return b;
}

int main(){
    int in;
    int counter = 0;

    int start = 0;
    int dp[100000];
    int hight[100000];
    scanf("%d", &hight[0]);
    dp[0] = max(hight[0], 0);
    int ans = dp[0];
    while(scanf("%d", &in) != EOF){
        counter++;
        hight[counter] = in;
        if(dp[counter-1] + in > 0){
            dp[counter] = dp[counter-1] + in;
            ans = max(ans, dp[counter]);
        }
        else{
            dp[counter] = 0;
            start = counter+1;
        }
    }
    if(dp[counter] == 0 || start == 0)
        printf("%d",ans);
    else{
        dp[0] = hight[0]+dp[counter];
        for(int i = 1; i < start; i++){
            if(dp[i-1]+hight[i] <= dp[i])
                break;
            else{
                dp[i] = dp[i-1]+hight[i];
                ans = max(ans, dp[i]);
            }
        }
        printf("%d\n",ans);
    }
}