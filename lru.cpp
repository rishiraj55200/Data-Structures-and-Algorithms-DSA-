

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