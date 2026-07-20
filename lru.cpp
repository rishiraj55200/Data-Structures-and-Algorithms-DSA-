

class Node {
public:
    int key;
    int val;
    Node* prev;
    Node* next;

    Node(int k, int v) {
        key = k;
        val = v;
        prev = NULL;
        next = NULL;
    }
};

class LRUCache {
public:
    int capacity;
    unordered_map<int, Node*> mp;
    Node* head;
    Node* tail;

    LRUCache(int cap) {
        capacity = cap;

        head = new Node(-1, -1); // dummy head
        tail = new Node(-1, -1); // dummy tail

        head->next = tail;
        tail->prev = head;
    }

    void removeNode(Node* node) {
        Node* p = node->prev;
        Node* n = node->next;

        p->next = n;
        n->prev = p;
    }

    void insertFront(Node* node) {
        node->next = head->next;
        node->prev = head;

        head->next->prev = node;
        head->next = node;
    }

    int get(int key) {
        if (mp.find(key) == mp.end())
            return -1;

        Node* node = mp[key];

        removeNode(node);
        insertFront(node);

        return node->val;
    }

    void put(int key, int value) {

        if (mp.find(key) != mp.end()) {
            Node* node = mp[key];

            node->val = value;

            removeNode(node);
            insertFront(node);
        }
        else {

            if ((int)mp.size() == capacity) {

                Node* lru = tail->prev;

                removeNode(lru);
                mp.erase(lru->key);

                delete lru;
            }

            Node* node = new Node(key, value);

            insertFront(node);
            mp[key] = node;
        }
    }
};

// production level code
/*

#include <iostream>
#include <unordered_map>
using namespace std;

// ---------------------------------------------------------
// LRU CACHE (Least Recently Used Cache)
//
// Requirement: get(key) and put(key, val) dono O(1) time me hone chahiye.
// Jab cache full ho jaye aur naya item daalna ho, to sabse "purani use hui"
// (least recently used) key ko hata do.
//
// Idea:
//   1) Doubly Linked List -> order maintain karne ke liye
//        (front = most recently used, back = least recently used)
//   2) HashMap (key -> Node*) -> O(1) me kisi bhi node tak seedha pahuchne ke liye
//
// Dono ko combine karne se get/put O(1) ho jaate hain.
// ---------------------------------------------------------

class Node {
public:
    int key;
    int val;
    Node* prev;
    Node* next;

    Node(int key, int val) {
        this->key = key;
        this->val = val;
        this->prev = NULL;
        this->next = NULL;
    }
};

class LRUCache {
private:
    int capacity;                     // cache ki max size
    unordered_map<int, Node*> mp;     // key -> uss key ka node (O(1) lookup ke liye)

    // Dummy head aur dummy tail rakhe hain taaki edge cases
    // (list empty hai, ya sirf 1 node hai) alag se handle na karne pade.
    // head ke turant baad wala node = MOST recently used
    // tail ke turant pehle wala node = LEAST recently used
    Node* head;
    Node* tail;

    // Kisi node ko uski current position se hata do (list se detach)
    // Isko hum tab use karenge jab: (a) node ko move karna ho, (b) node ko delete karna ho
    void removeNode(Node* node) {
        Node* prevNode = node->prev;
        Node* nextNode = node->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;
    }

    // Node ko hamesha "head ke turant baad" insert karo
    // (matlab ye node ab MOST recently used ban gaya)
    void insertAtFront(Node* node) {
        Node* firstRealNode = head->next;

        head->next = node;
        node->prev = head;

        node->next = firstRealNode;
        firstRealNode->prev = node;
    }

public:
    LRUCache(int capacity) {
        this->capacity = capacity;

        // dummy nodes banaye - inme koi real key/val nahi hota
        head = new Node(-1, -1);
        tail = new Node(-1, -1);

        // shuru me list empty hai, to head aur tail ek dusre se connected hain
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        // agar key exist nahi karti
        if (mp.find(key) == mp.end()) {
            return -1;
        }

        Node* node = mp[key];
        int result = node->val;

        // is key ko "abhi use kiya gaya" maana jayega,
        // isliye ise list se nikal ke wapas front pe daal do
        removeNode(node);
        insertAtFront(node);

        return result;
    }

    void put(int key, int val) {
        // Case 1: key already maujood hai -> value update karo
        // aur usko most recently used bana do (front pe le aao)
        if (mp.find(key) != mp.end()) {
            Node* node = mp[key];
            node->val = val;

            removeNode(node);
            insertAtFront(node);
            return;
        }

        // Case 2: cache already full hai -> least recently used node hatao
        // (least recently used node hamesha tail ke turant pehle wala hota hai)
        if ((int)mp.size() == capacity) {
            Node* lruNode = tail->prev;

            removeNode(lruNode);
            mp.erase(lruNode->key);   // hashmap se bhi entry hatani zaroori hai
            delete lruNode;            // memory free karo, warna leak ho jayega
        }

        // Case 3: naya node banao, front me daalo, hashmap me bhi entry karo
        Node* newNode = new Node(key, val);
        insertAtFront(newNode);
        mp[key] = newNode;
    }

    // ---------- DESTRUCTOR ----------
    // Poori linked list traverse karke sab kuch delete karo (dummy nodes samet)
    ~LRUCache() {
        Node* curr = head;
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
};

int main() {
    LRUCache cache(2);   // capacity = 2

    cache.put(1, 10);
    cache.put(2, 20);
    cout << cache.get(1) << endl;   // 10  (1 ab most recently used ban gaya)

    cache.put(3, 30);               // capacity full thi, 2 sabse purana tha -> 2 evict ho gaya
    cout << cache.get(2) << endl;   // -1  (2 evict ho chuka hai)

    cache.put(4, 40);               // capacity full, is baar 1 sabse purana hai -> 1 evict hoga
    cout << cache.get(1) << endl;   // -1  (1 evict ho chuka hai)
    cout << cache.get(3) << endl;   // 30
    cout << cache.get(4) << endl;   // 40

    return 0;
}

*/