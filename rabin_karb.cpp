/*
    RABIN-KARP STRING MATCHING ALGORITHM
    -------------------------------------
    GOAL: Find all occurrences of a "pattern" string inside a "text" string.

    CORE IDEA:
    - Instead of comparing pattern with every substring of text character by character (O(n*m)),
      we compute a HASH of the pattern and a HASH of every window (substring of same length) in text.
    - If hash of window == hash of pattern -> it's a POSSIBLE match (could be hash collision,
      called a "spurious hit") -> so we do one final char-by-char check to confirm.
    - We use ROLLING HASH so that hash of next window can be computed in O(1) from previous window's hash,
      instead of recomputing from scratch. This is the key trick that makes it fast: O(n + m) average case.

    ROLLING HASH FORMULA (treat string like a number in base 'd', mod a large prime 'q'):
        hash(s[0..m-1]) = ( s[0]*d^(m-1) + s[1]*d^(m-2) + ... + s[m-1]*d^0 ) mod q

    To slide window by 1 (remove leftmost char, add new right char):
        new_hash = ( d * ( old_hash - text[i]*d^(m-1) ) + text[i+m] ) mod q

    where:
        d  = number of characters in alphabet (usually 256 for extended ASCII)
        q  = a large prime number (reduces chance of collisions, keeps hash values manageable)
        m  = length of pattern
        n  = length of text

    TIME COMPLEXITY:
        - Average / best case: O(n + m)   -> hashing is O(1) per shift
        - Worst case: O(n*m)              -> happens when there are many spurious hits
                                              (hash collisions), forcing char-by-char checks often
    SPACE COMPLEXITY: O(1) extra (excluding input strings)

    USE CASE: Plagiarism detection, substring search, finding repeated DNA sequences, etc.
*/

#include <bits/stdc++.h>
using namespace std;

// d = number of characters in input alphabet (256 for ASCII)
#define d 256

void rabinKarpSearch(string pattern, string text, int q) {
    int m = pattern.size();
    int n = text.size();

    int i, j;
    int p = 0;   // hash value for pattern
    int t = 0;   // hash value for current window of text
    int h = 1;   // h will store d^(m-1) % q  -> needed to remove leading digit while sliding

    // -------------------------------------------------
    // STEP 1: Precompute h = d^(m-1) % q
    // (this is the "weight" of the leftmost character in the window)
    // -------------------------------------------------
    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    // -------------------------------------------------
    // STEP 2: Compute initial hash value of pattern and of first window of text
    // -------------------------------------------------
    for (i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    // -------------------------------------------------
    // STEP 3: Slide the pattern over text one by one
    // -------------------------------------------------
    for (i = 0; i <= n - m; i++) {

        // If hash values match, then only check characters one by one
        // (this guards against spurious hits due to hash collisions)
        if (p == t) {
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j])
                    break;
            }

            if (j == m) {
                cout << "Pattern found at index " << i << endl;
            }
        }

        // ---------------------------------------------
        // STEP 4: Compute hash value for next window of text
        // Remove leading digit, add trailing digit
        // ---------------------------------------------
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;

            // We might get negative value of t, converting it to positive
            if (t < 0)
                t = (t + q);
        }
    }
}

int main() {
    string text = "ABCEABCDABCEABF";
    string pattern = "ABCE";

    int q = 101;   // a prime number, used as modulus (keeps hash values small)
    // Larger primes reduce collision probability but slightly increase computation

    rabinKarpSearch(pattern, text, q);

    return 0;
}

/*
    NOTES / QUICK REVISION POINTS:
    --------------------------------
    1. Rabin-Karp uses HASHING + ROLLING HASH to avoid recomputation.
    2. Always verify hash match with actual character comparison (avoid false positives).
    3. Choice of prime 'q': should be large enough to minimize collisions but not overflow int.
       (Real implementations often use long long or modular arithmetic with big primes.)
    4. 'h' = d^(m-1) % q is precomputed ONCE — represents the place value of the
       most significant character in the window (like the "thousands place" in a number).
    5. Rolling hash update logic:
            remove old char's contribution -> multiply by base -> add new char
       This mimics how you'd update a number when shifting digits.
    6. Worst case O(n*m) happens if hash function is poor and many collisions occur
       (e.g., very small q, or patterned text like "aaaaaaa...").
    7. Compare with:
        - Naive substring search: O((n-m+1)*m) always
        - KMP algorithm: O(n+m) worst case guaranteed (uses prefix function, no hashing)
        - Rabin-Karp: good for MULTIPLE pattern search too (compute hash set of patterns,
          then just check text hashes against that set) — this is where it shines over KMP.
    8. Real-world variant: use double hashing (two different mod values) to make
       collision probability negligible — common in competitive programming.
*/