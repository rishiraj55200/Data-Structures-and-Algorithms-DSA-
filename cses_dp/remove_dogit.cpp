#include<bits/stdc++.h>
using namespace std;

 int solve(int n , vector<int>&dp){
    if(n == 0)return 0;

    if(n < 10)return 1;
   
    if(dp[n] != -1)return dp[n];

    int ans = INT_MAX;
    int temp = n;
    while(temp != 0){
        int rem = temp%10;
        temp = temp/10;
        if(rem == 0)continue;
        int next = 1 + solve(n-rem,dp);
        ans = min(ans, next);

    }

    return dp[n] = ans;
 }
int main(){

     //cout << "enter number n" << endl;
     int n ;
     cin >> n;
    
     vector<int>dp(n+1 , -1);
     int ans = solve(n , dp);

     cout << ans << endl;

    
    
    return 0;
}