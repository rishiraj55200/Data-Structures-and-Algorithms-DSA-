#include <bits/stdc++.h>
using namespace std;

/*
    CONSTRUCT BINARY TREE FROM POSTORDER + INORDER TRAVERSAL
    ------------------------------------------------------
    REVISION NOTES:

    1) KEY IDEA
       - Postorder = [left subtree..., right subtree..., ROOT]
         -> the LAST element of postorder is ALWAYS the root of the
            (sub)tree we're currently building.
       - Inorder   = [left subtree..., ROOT, right subtree...]
         -> find that root in inorder. Left of it = left subtree,
            right of it = right subtree (same as preorder version).

    2) THE FLIP vs PREORDER VERSION
       - In preorder+inorder, we consumed preorder LEFT to RIGHT and
         built LEFT subtree before RIGHT.
       - Here, postorder must be consumed RIGHT to LEFT (from the
         back), because the root sits at the END, and right before
         it (going backwards) is the last node of the RIGHT subtree.
       - So: build RIGHT subtree FIRST, then LEFT subtree.
         This is the #1 source of bugs - flipping this order breaks
         the whole tree.

    3) POINTER MOVEMENT
       - postIdx starts at postorder.size() - 1 (points at root of
         the WHOLE tree first).
       - Each call: take postorder[postIdx] as root, postIdx--,
         THEN recurse right first, then left.

    4) HASHMAP - same trick as before
       - unordered_map<val, index in inorder> for O(1) root lookup.
       - Precomputed once -> O(n) total instead of O(n^2).
       - Assumes all node values are unique.

    5) BASE CASE
       - Bound each call with (inStart, inEnd), the inorder range the
         CURRENT subtree must occupy. inStart > inEnd -> return NULL.

    6) COMPLEXITY
       - Time:  O(n)
       - Space: O(n)  (map + recursion stack, O(n) worst case skewed tree)
*/

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
private:
    unordered_map<int, int> inIndex;   // value -> its index in inorder[]
    vector<int> postorder;
    int postIdx;                       // pointer into postorder, moves RIGHT->LEFT

    // builds the subtree that must occupy inorder[inStart..inEnd]
    TreeNode* build(int inStart, int inEnd) {
        if (inStart > inEnd) return NULL;            // no nodes in this range

        int rootVal = postorder[postIdx];            // last unused postorder value = root
        postIdx--;                                    // consume it NOW, before recursing
        TreeNode* root = new TreeNode(rootVal);

        int mid = inIndex[rootVal];                   // root's position in inorder

        // IMPORTANT: build RIGHT first - postorder gives us all of the
        // right subtree's nodes (from the back) before the left subtree's
        root->right = build(mid + 1, inEnd);
        root->left  = build(inStart, mid - 1);

        return root;
    }

public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorderIn) {
        postorder = postorderIn;
        postIdx = (int)postorder.size() - 1;          // start from the END
        for (int i = 0; i < (int)inorder.size(); i++) {
            inIndex[inorder[i]] = i;                   // precompute for O(1) lookup
        }
        return build(0, (int)inorder.size() - 1);
    }
};

// ---- quick sanity check ----
void printPreorder(TreeNode* root) {
    if (!root) return;
    cout << root->val << " ";
    printPreorder(root->left);
    printPreorder(root->right);
}

int main() {
    // tree:        3
    //            /   \
    //           9     20
    //                /  \
    //               15   7
    vector<int> inorder   = {9, 3, 15, 20, 7};
    vector<int> postorder = {9, 15, 7, 20, 3};

    Solution sol;
    TreeNode* root = sol.buildTree(inorder, postorder);

    cout << "Preorder of constructed tree: ";
    printPreorder(root);   // should print: 3 9 20 15 7
    cout << "\n";
    return 0;
}

/*
    REVISION - 10 SECOND RECAP (say this out loud):
    "Postorder's LAST element = root. Find it in inorder via hashmap -
     left of it is left subtree, right of it is right subtree. Move
     the postorder pointer backwards, and recurse RIGHT subtree first,
     then LEFT - because postorder lists left, then right, then root,
     so reading backwards you hit right-subtree nodes before left."

    SIDE-BY-SIDE MENTAL MODEL (this is what trips people up):
      preorder + inorder  -> root = FIRST of preorder -> build LEFT then RIGHT
      postorder + inorder -> root = LAST of postorder  -> build RIGHT then LEFT
      (the two are mirror images of each other)

    COMMON FOLLOW-UPS:
    - "Can you construct a tree from preorder + postorder alone (no
      inorder)?" -> only works if the tree is a FULL binary tree
      (every node has 0 or 2 children); otherwise ambiguous.
    - "What if duplicate values exist?" -> hashmap lookup breaks;
      would need node identity info beyond raw values.
    - "Iterative version instead of recursion?" -> doable with an
      explicit stack, but recursive is what's expected unless asked.
*/