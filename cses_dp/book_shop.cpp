#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, x;
    cin >> n >> x;

    vector<int> price(n), pages(n);

    for (int i = 0; i < n; i++) cin >> price[i];
    for (int i = 0; i < n; i++) cin >> pages[i];

    // dp[j] = max pages with budget j
    vector<int> dp(x + 1, 0);

    for (int i = 0; i < n; i++) {
        // traverse backwards (IMPORTANT for 0/1 knapsack)
        for (int j = x; j >= price[i]; j--) {
            dp[j] = max(dp[j], pages[i] + dp[j - price[i]]);
        }
    }

    cout << dp[x] << "\n";

    return 0;
}