#include<bits/stdc++.h>
using namespace std;
const int mod = 1e9+7;

int solve(int amount , vector<int>&arr , vector<int>&dp){
    if(amount == 0)return 1;
    if(amount < 0)return 0;

    if(dp[amount] != -1)return dp[amount];
    int ans = 0;

    for(auto it : arr){
        int temp = solve(amount - it , arr , dp);
        ans  = (ans + temp)%mod;
    }

    return dp[amount] = ans;
}

int main(){
    
    int n;
    int amount;
    cin >> n;
    cin >> amount;

    vector<int>dp(amount+1 , -1);

    vector<int>arr(n);
    for(int i = 0; i<n; i++){
        cin >> arr[i];
    }

    cout << solve(amount , arr , dp);
    
    return 0;
}