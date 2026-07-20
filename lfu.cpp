#include <bits/stdc++.h>
using namespace std;

/*
    LFU CACHE - O(1) get and put
    ------------------------------------------------------
    LRU evicts by recency alone. LFU needs recency + frequency:
      -> evict the Least Frequently Used key
      -> on a tie in frequency, evict the Least Recently Used among them

    DATA STRUCTURES:
    1) unordered_map<key, Node*> keyNode
         -> O(1) lookup of any key's node

    2) unordered_map<freq, DLL*> freqList
         -> each frequency has its OWN doubly linked list
         -> within one freq-bucket, the list behaves like a mini LRU:
            front = most recently used at this freq, back = least recently used

    3) minFreq
         -> tracks the current minimum frequency present in the cache
         -> lets eviction be O(1): no scanning needed, just look at
            freqList[minFreq] and remove its last node

    WHY THIS WORKS:
    Every time a key is touched (get OR put-on-existing-key), its freq
    increases by 1. So it gets removed from freqList[oldFreq] and
    pushed to the FRONT of freqList[oldFreq + 1] (since it's now the
    most-recently-used entry at that new frequency).
*/

class Node {
public:
    int key, val, freq;
    Node *prev, *next;
    Node(int k, int v) {
        key = k; val = v;
        freq = 1;              // every brand-new key starts at freq = 1
        prev = next = NULL;
    }
};

// Doubly linked list with dummy head/tail -> holds all keys at ONE frequency
class DLL {
public:
    Node *head, *tail;
    int size;                  // count of real nodes in this bucket

    DLL() {
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    void insertFront(Node* n) {
        Node* temp = head->next;
        head->next = n;
        n->prev = head;
        n->next = temp;
        temp->prev = n;
        size++;
    }

    void remove(Node* n) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        size--;
    }

    // LRU node of THIS frequency bucket always sits right before tail
    Node* removeLast() {
        if (size == 0) return NULL;
        Node* n = tail->prev;
        remove(n);
        return n;
    }
};

class LFUCache {
private:
    int capacity;
    int minFreq;
    unordered_map<int, Node*> keyNode;   // key  -> node
    unordered_map<int, DLL*> freqList;   // freq -> bucket of nodes at that freq

    // bump a node's frequency by 1 and relocate it to the new bucket
    void touch(Node* n) {
        int oldFreq = n->freq;
        freqList[oldFreq]->remove(n);

        // if we just emptied the bucket that WAS the min freq, bump minFreq
        if (freqList[oldFreq]->size == 0 && minFreq == oldFreq) {
            minFreq++;
        }

        n->freq++;
        if (freqList.find(n->freq) == freqList.end()) {
            freqList[n->freq] = new DLL();
        }
        freqList[n->freq]->insertFront(n);
    }

public:
    LFUCache(int cap) {
        capacity = cap;
        minFreq = 0;
    }

    int get(int key) {
        if (keyNode.find(key) == keyNode.end()) return -1;
        Node* n = keyNode[key];
        int ans = n->val;
        touch(n);              // a get() counts as an access -> freq++
        return ans;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;   // guard: no-op cache

        // case 1: key already present -> update value, bump freq
        if (keyNode.find(key) != keyNode.end()) {
            Node* n = keyNode[key];
            n->val = value;
            touch(n);
            return;
        }

        // case 2: cache full -> evict LRU node from the min-freq bucket
        if ((int)keyNode.size() == capacity) {
            Node* victim = freqList[minFreq]->removeLast();
            keyNode.erase(victim->key);
            delete victim;
        }

        // case 3: insert the new key fresh, always at freq = 1
        Node* newNode = new Node(key, value);
        keyNode[key] = newNode;
        if (freqList.find(1) == freqList.end()) {
            freqList[1] = new DLL();
        }
        freqList[1]->insertFront(newNode);
        minFreq = 1;   // any brand-new insert resets minFreq to 1
    }

    ~LFUCache() {
        for (auto& p : keyNode) delete p.second;
        for (auto& p : freqList) {
            delete p.second->head;
            delete p.second->tail;
            delete p.second;
        }
    }
};

int main() {
    LFUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    cout << cache.get(1) << "\n";   // 1, freq(1)=2
    cache.put(3, 3);                // evicts key 2 (freq(2)=1 is min, key 1 freq=2)
    cout << cache.get(2) << "\n";   // -1
    cout << cache.get(3) << "\n";   // 3
    return 0;
}