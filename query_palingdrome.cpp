/*
    PALINDROME SUBSTRING CHECK USING ROLLING HASH (DOUBLE HASHING)
    -----------------------------------------------------------------
    PROBLEM: Given a string s and multiple queries (left, right),
    tell whether s[left..right] is a palindrome or not.

    NAIVE WAY: For each query, use two pointers and check char by char
               -> O(length) per query -> slow if many queries.

    SMART WAY (this code): Use ROLLING HASH.
        - A substring s[l..r] is a palindrome
          IF AND ONLY IF
          hash(s[l..r])  ==  hash(reverse of s[l..r])

        - Instead of reversing substring every time, we PRE-REVERSE
          the whole string once, and pre-compute hashes for BOTH
          the original string and the reversed string.

        - Then for any query (l, r):
            forward_hash  = hash of s[l..r]              (from original string)
            backward_hash = hash of s[l..r] reversed      (from reversed string,
                                                            just at mirrored indices)

          If forward_hash == backward_hash -> palindrome, else not.

    WHY DOUBLE HASHING (2 different mod/base pairs)?
        - Single hash can have collisions (two different strings giving
          same hash by chance) -> wrong answer.
        - Using TWO independent hashes together makes collision
          probability extremely small (practically negligible).
        - So we always compare BOTH hash values together as a pair.

    TIME COMPLEXITY:
        - Precompute all hashes: O(n)
        - Each query answered in: O(1)
        - Total for n length string, q queries: O(n + q)
*/

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1e9 + 7;
const long long BASE1 = 31;
const long long BASE2 = 37;

/*
    We store hash arrays as plain vectors (no class), simpler to trace
    while revising.

    hash1[i] = hash of prefix s[0..i-1] using BASE1
    hash2[i] = hash of prefix s[0..i-1] using BASE2

    power1[i] = BASE1^i % MOD   (needed to shift hash values while combining)
    power2[i] = BASE2^i % MOD
*/

struct HashArrays {
    vector<long long> hash1, hash2, power1, power2;

    void build(const string& s) {
        int n = s.size();
        hash1.assign(n + 1, 0);
        hash2.assign(n + 1, 0);
        power1.assign(n + 1, 1);
        power2.assign(n + 1, 1);

        for (int i = 0; i < n; i++) {
            // hash of prefix ending at i+1 = (prev_hash * base + current_char) % MOD
            hash1[i + 1] = (hash1[i] * BASE1 + s[i]) % MOD;
            hash2[i + 1] = (hash2[i] * BASE2 + s[i]) % MOD;

            // precompute powers of base, used later to align hash values
            power1[i + 1] = (power1[i] * BASE1) % MOD;
            power2[i + 1] = (power2[i] * BASE2) % MOD;
        }
    }

    // returns hash of substring s[left..right] (both inclusive), 0-indexed
    pair<long long,long long> getHash(int left, int right) {
        int len = right - left + 1;

        // formula: hash(l,r) = hash[r+1] - hash[l] * base^len   (mod MOD)
        long long h1 = (hash1[right + 1] - (hash1[left] * power1[len]) % MOD + MOD) % MOD;
        long long h2 = (hash2[right + 1] - (hash2[left] * power2[len]) % MOD + MOD) % MOD;

        return {h1, h2};
    }
};

vector<int> palQueries(string s, vector<vector<int>>& queries) {
    int n = s.size();

    string revS = s;
    reverse(revS.begin(), revS.end());

    HashArrays forwardHash, reverseHash;
    forwardHash.build(s);      // hashes of original string
    reverseHash.build(revS);   // hashes of reversed string

    vector<int> result;

    for (auto& q : queries) {
        int left = q[0];
        int right = q[1];

        // hash of s[left..right] in the ORIGINAL string
        auto h1 = forwardHash.getHash(left, right);

        /*
           Same substring, but we need its hash from the REVERSED string,
           to compare against reverse(s[left..right]).

           Mapping index from original string to reversed string:
               original index i  -->  reversed index (n - 1 - i)

           So s[left..right] reversed = revS[(n-1-right) .. (n-1-left)]
        */
        int revLeft = n - 1 - right;
        int revRight = n - 1 - left;
        auto h2 = reverseHash.getHash(revLeft, revRight);

        // palindrome only if BOTH hash values match
        if (h1 == h2)
            result.push_back(1);
        else
            result.push_back(0);
    }

    return result;
}

int main() {
    string str = "abaaabaaaba";
    vector<vector<int>> queries = {{0, 10}, {5, 8}, {2, 5}, {5, 9}};

    vector<int> res = palQueries(str, queries);

    for (auto i : res) cout << i << " ";
    cout << endl;

    return 0;
}

/*
    REVISION NOTES:
    -----------------
    1. Core trick: palindrome check = compare hash(substring) with
       hash(reverse of substring). No need to actually reverse each query.

    2. Prefix hash formula (polynomial hashing):
           hash[i+1] = hash[i]*base + s[i]   (mod M)
       This treats string as a number in given base.

    3. To get hash of substring [l, r] from prefix hashes:
           hash(l,r) = hash[r+1] - hash[l] * base^(r-l+1)   (mod M)
       (Same idea as: number formed by digits l..r = 
        remove the higher part's contribution using base power shift)

    4. Why do we precompute "power" array separately?
       Because base^len is needed every query, precomputing avoids
       recalculating pow(base, len) again and again -> O(1) per query.

    5. DOUBLE HASHING (base1,mod) + (base2,mod) used together
       reduces the chance of hash collision to near zero.
       -> Rule of thumb: always use 2 hashes in competitive programming
          when correctness matters (single hash can be hacked/collided).

    6. Index mapping between original and reversed string is the
       trickiest part — just remember:
           original index i  <->  reversed index (n-1-i)

    7. Overall complexity:
           Build hashes: O(n) for each string (done once)
           Answer each query: O(1)
           Total: O(n + q)

    8. Compare with brute-force:
           Brute force palindrome check per query = O(length of substring)
           -> Bad if many queries or long strings.
           Rolling hash approach = O(1) per query after O(n) preprocessing.
*/