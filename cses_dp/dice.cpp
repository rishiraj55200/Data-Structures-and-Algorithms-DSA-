#include<bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;
int find_ans( int sum , vector<int>&dp){
   
    if(sum  == 0)return 1;

    if(dp[sum] != -1)return dp[sum];

    //if(sum < 0)return 0;
    int ans= 0;

    for(int i = 1; i<=6; i++){
         if(sum - i >= 0){
            ans = (ans + find_ans(sum -i ,dp))%mod;
         }
        
        
    }

    dp[sum] = ans;
    return ans;

}
int main(){
     
    int n;
    cin >> n;
    vector<int>dp(n+1 , -1);

    cout << find_ans(n,dp);

    
    
    return 0;
}