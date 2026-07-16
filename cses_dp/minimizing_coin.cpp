#include <bits/stdc++.h>
using namespace std;

int solve(int amount, vector<int>& coins, vector<int>& dp){
    if(amount == 0) return 0;
    if(dp[amount] != -1) return dp[amount];

    int ans = INT_MAX;

    for(int coin : coins){
        if(amount >= coin){
            int temp = solve(amount - coin, coins, dp);
            if(temp != INT_MAX){
                ans = min(ans, 1 + temp);
            }
        }
    }

    return dp[amount] = ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, amount;
    cin >> n >> amount;

    vector<int> coins(n);
    for(int i = 0; i < n; i++) cin >> coins[i];

    vector<int> dp(amount + 1, -1);

    int ans = solve(amount, coins, dp);

    cout << (ans == INT_MAX ? -1 : ans);
}