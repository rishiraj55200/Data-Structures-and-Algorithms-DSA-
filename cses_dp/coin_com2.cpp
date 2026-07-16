#include<bits/stdc++.h>
using namespace std;

const int mod = 1e9+7;

int main(){
    int n, amount;
    cin >> n >> amount;

    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }

    vector<int> dp(amount+1, 0);
    dp[0] = 1;

    for(int coin : arr){
        for(int j = coin; j <= amount; j++){
            dp[j] = (dp[j] + dp[j - coin]) % mod;
        }
    }

    cout << dp[amount];

    return 0;
}