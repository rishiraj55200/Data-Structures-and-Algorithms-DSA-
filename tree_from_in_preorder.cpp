#include <bits/stdc++.h>
using namespace std;

/*
    CONSTRUCT BINARY TREE FROM PREORDER + INORDER TRAVERSAL
    ------------------------------------------------------
    REVISION NOTES (read this before the code):

    1) KEY IDEA
       - Preorder = [ROOT, left subtree..., right subtree...]
         -> the FIRST element of preorder is ALWAYS the root of the
            (sub)tree we're currently building.
       - Inorder  = [left subtree..., ROOT, right subtree...]
         -> once we know the root (from preorder), find it in inorder.
            Everything to its LEFT in inorder = left subtree.
            Everything to its RIGHT in inorder = right subtree.

    2) WHY WE NEED A HASHMAP
       - Naively searching for root's position in inorder is O(n)
         per call -> O(n^2) overall.
       - Precompute unordered_map<val, index in inorder> ONCE at the
         start -> O(1) lookup -> overall O(n).
       (Assumes all node values are unique - standard assumption
        for this problem, mention it if interviewer asks.)

    3) THE TRICKY PART: MOVING THE PREORDER POINTER
       - We use a single global/shared index `preIdx` that always
         points to "the next unused element of preorder".
       - We increment `preIdx` the MOMENT we consume a value as a
         root - BEFORE recursing left. This matters because:
           preorder order is [root, left..., right...]
         so after taking root, the very next preorder element
         belongs to the LEFT subtree, and recursion must consume
         the entire left subtree's preorder values before it
         ever reaches the right subtree's values.
       - This is the #1 source of bugs when rewriting this from
         memory - people forget WHEN to increment preIdx.

    4) BASE CASE
       - We bound each recursive call with (inStart, inEnd) - the
         range in inorder that the CURRENT subtree must occupy.
       - If inStart > inEnd -> no nodes left -> return NULL.

    5) COMPLEXITY
       - Time:  O(n)  (map build O(n) + each node processed once O(1))
       - Space: O(n)  (map + recursion stack up to O(h), worst O(n))
*/

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
private:
    unordered_map<int, int> inIndex;   // value -> its index in inorder[]
    vector<int> preorder;
    int preIdx = 0;                    // pointer into preorder, moves left->right

    // builds the subtree that must occupy inorder[inStart..inEnd]
    TreeNode* build(int inStart, int inEnd) {
        if (inStart > inEnd) return NULL;          // no nodes in this range

        int rootVal = preorder[preIdx];            // next unused preorder value = root
        preIdx++;                                   // consume it NOW, before recursing
        TreeNode* root = new TreeNode(rootVal);

        int mid = inIndex[rootVal];                 // root's position in inorder

        // IMPORTANT: build LEFT first - because preorder gives us all of
        // left subtree's nodes before any of right subtree's nodes
        root->left  = build(inStart, mid - 1);
        root->right = build(mid + 1, inEnd);

        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorderIn, vector<int>& inorder) {
        preorder = preorderIn;
        preIdx = 0;
        for (int i = 0; i < (int)inorder.size(); i++) {
            inIndex[inorder[i]] = i;                // precompute for O(1) lookup
        }
        return build(0, (int)inorder.size() - 1);
    }
};

// ---- quick sanity check ----
void printInorder(TreeNode* root) {
    if (!root) return;
    printInorder(root->left);
    cout << root->val << " ";
    printInorder(root->right);
}

int main() {
    // tree:        3
    //            /   \
    //           9     20
    //                /  \
    //               15   7
    vector<int> preorder = {3, 9, 20, 15, 7};
    vector<int> inorder  = {9, 3, 15, 20, 7};

    Solution sol;
    TreeNode* root = sol.buildTree(preorder, inorder);

    cout << "Inorder of constructed tree: ";
    printInorder(root);   // should print: 9 3 15 20 7
    cout << "\n";
    return 0;
}

/*
    REVISION - 10 SECOND RECAP (say this out loud when revising):
    "Preorder's first element = root. Find that root in inorder using
     a hashmap - everything left of it in inorder is the left subtree,
     everything right is the right subtree. Recurse left before right,
     because preorder lists the entire left subtree before touching
     the right subtree. Advance the preorder pointer the instant you
     use a value as a root."

    COMMON FOLLOW-UPS:
    - "Do it with postorder + inorder instead of preorder + inorder"
        -> postorder = [left..., right..., ROOT], so the root is the
           LAST element. You then must recurse RIGHT subtree first,
           then LEFT (walk preIdx backwards from the end).
    - "Can you do it without a hashmap?" -> yes but O(n^2) worst case
      (linear search for root in inorder each time).
    - "What if values aren't unique?" -> this technique breaks (map
      lookup ambiguous); need extra info like node indices tagged in
      input, or a different traversal pair.
    - "Space complexity concern?" -> mention hashmap O(n) + recursion
      stack O(h) (O(n) worst case for skewed tree, O(log n) balanced).
*/