#include<bits/stdc++.h>
using namespace std;

/*   taks : implement hash map with spearating chaining for collision
   1 > o(1) insert (key , val)
   2 > o(1) find (key) get val
   3 > delete (key)

   HashMap using Separate Chaining - Quick Revision
Hash function se key ko bucket index me convert karte hain:
index = ((key % size) + size) % size;
Har bucket ek linked list ka head store karti hai.
Insert: bucket me jao, key mil jaye to value update karo, warna new node head par insert karo.
Search: bucket ki linked list traverse karo aur key milne par value return karo.
Delete: linked list me key dhoondo, node ko unlink karke delete kar do.
Average TC = O(1), Worst TC = O(n) (jab saari keys same bucket me aa jayein).




*/ 

class node {
    public :
       int key ;
       int val ;
       node* next;

       node(int key , int val){
        this->key = key;
        this->val = val;
        this->next = NULL;
       }
};

class hash_map{
      private:
          int size ;
          vector<node*>hash;
         
          
      public:


        hash_map(){
            size = 1000;
            hash.resize(1000 , NULL);
        }  

        int find_index(int key){

            return ((key % size) + size) % size;

        }

      void insert(int key , int val){
            int index =  find_index(key);
            node* curr = hash[index];

            while(curr != NULL){
                if(curr->key == key){
                    curr->val = val;
                    return;
                }
                curr = curr->next;
            }

            curr = hash[index];
            node* new_node = new node(key , val);
            new_node->next = curr;
            hash[index] = new_node;

      }

      int find_val(int key){
         int index = find_index(key);
         node* curr = hash[index];
         while(curr != NULL){
            if(curr->key == key){
                return curr->val;
            }

            curr = curr->next;
         }

         return -1;
      }

      void delete_key(int key){
        int index = find_index(key);
        node* curr = hash[index];
        node* prev = NULL;

        while(curr != NULL){
            if(curr->key == key){
                break;
            }
            prev = curr;
            curr = curr->next;

        }

        if(prev == NULL){
            hash[index] = curr->next;
        }
        else{
            prev->next = curr->next;
        }

        delete(curr);
      }

      ~hash_map(){

        for(int i = 0 ; i<size; i++){
            node* temp = hash[i];
            while(temp != NULL){
                node* curr = temp;
                temp = temp->next;
                delete(curr);
            }
        }
      }

    
      


};


 

int main(){
  

    hash_map* mp = new hash_map();
    mp->insert(10 , 20);
    mp->insert(30 , 40);
    cout << mp->find_val(10) << endl;
     mp->delete_key(10) ;
    cout << mp->find_val(10) << endl;



    



    
   
    return 0;
}

// production ready
/*
#include <iostream>
#include <vector>
using namespace std;

// ---------------------------------------------------------
// HashMap using vector<Node*> (bucket array) + separate chaining.
// Vector use karne ka fayda: vector khud apni memory manage karta hai
// (resize/delete[] manually karne ki zarurat nahi), aur Node** jaisa
// "pointer to pointer" wala confusion nahi hota. Sirf ek simple
// "array of pointers" samajhna hai: buckets[i] = us bucket ki
// linked list ka head node.
// ---------------------------------------------------------

class Node {
public:
    int key;
    int val;
    Node* next;   // next iss linked list ka agla node hai (collision chain)

    Node(int key, int val) {
        this->key = key;
        this->val = val;
        this->next = NULL;
    }
};

class HashMap {
private:
    vector<Node*> buckets;   // buckets[i] = i-th bucket ki linked list ka HEAD pointer
    int bucket_count;         // buckets.size() jitna hi hoga, alag variable rakha for clarity
    int elem_count;            // total kitne key-value pairs store hain

    const double MAX_LOAD_FACTOR = 0.75;

    // Custom hash function (std::hash use nahi kiya, apna banaya)
    int hashFunction(int key) {
        return ((key % bucket_count) + bucket_count) % bucket_count;
    }

    // Load factor threshold cross hone par table double karo aur
    // saare purane nodes ko naye (bade) bucket array me daal do
    void rehash() {
        vector<Node*> old_buckets = buckets;   // purana bucket array copy kar liya (ye sirf pointers copy honge, nodes nahi)
        int old_bucket_count = bucket_count;

        bucket_count = bucket_count * 2;
        buckets.assign(bucket_count, NULL);     // naya bada vector banaya, sab NULL se fill kiya

        // purane har bucket ki linked list traverse karke naye table me reinsert karo
        for (int i = 0; i < old_bucket_count; i++) {
            Node* curr = old_buckets[i];
            while (curr != NULL) {
                Node* next = curr->next;   // pehle next save kiya, kyunki niche curr->next overwrite hoga

                int newIndex = hashFunction(curr->key);
                curr->next = buckets[newIndex];   // naye bucket ke front me daal diya
                buckets[newIndex] = curr;

                curr = next;
            }
        }
        // old_buckets (vector) apne aap destruct ho jayega scope ke bahar -
        // ismein sirf pointers the, actual Node's already naye buckets me move ho chuke hain,
        // isliye unhe dobara delete karne ki zarurat nahi.
    }

public:
    HashMap(int initial_size = 16) {
        bucket_count = initial_size;
        elem_count = 0;
        buckets.assign(bucket_count, NULL);   // vector ko initial_size length ka bana diya, sab NULL
    }

    // ---------- INSERT ----------
    void insert(int key, int val) {
        int index = hashFunction(key);
        Node* curr = buckets[index];

        // Agar key already maujood hai to sirf value update kar do
        while (curr != NULL) {
            if (curr->key == key) {
                curr->val = val;
                return;
            }
            curr = curr->next;
        }

        // Naya node banao aur bucket ke front me daal do (O(1) insert)
        Node* newNode = new Node(key, val);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        elem_count++;

        double currentLoadFactor = (double)elem_count / bucket_count;
        if (currentLoadFactor > MAX_LOAD_FACTOR) {
            rehash();
        }
    }

    // ---------- GET ----------
    // found ko reference se pass kiya taaki "-1 sentinel bug" na ho
    int get(int key, bool &found) {
        int index = hashFunction(key);
        Node* curr = buckets[index];

        while (curr != NULL) {
            if (curr->key == key) {
                found = true;
                return curr->val;
            }
            curr = curr->next;
        }

        found = false;
        return -1;
    }

    // ---------- DELETE ----------
    bool remove(int key) {
        int index = hashFunction(key);
        Node* curr = buckets[index];
        Node* prev = NULL;

        while (curr != NULL && curr->key != key) {
            prev = curr;
            curr = curr->next;
        }

        if (curr == NULL) {
            return false;   // key mili hi nahi
        }

        if (prev == NULL) {
            buckets[index] = curr->next;   // pehla node hi tha
        } else {
            prev->next = curr->next;
        }

        delete curr;
        elem_count--;
        return true;
    }

    int size() {
        return elem_count;
    }

    int getBucketCount() {
        return bucket_count;
    }

    // ---------- DESTRUCTOR ----------
    // vector khud apni memory (bucket array) free kar dega,
    // lekin Node* jo humne 'new' se banaye the wo manually delete karne padenge -
    // warna memory leak ho jayega (vector sirf pointers store karta hai, unke
    // pointed-to objects ko nahi jaanta).
    ~HashMap() {
        for (int i = 0; i < bucket_count; i++) {
            Node* curr = buckets[i];
            while (curr != NULL) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        // buckets (vector) apne aap destruct ho jayega, alag se delete[] nahi karna
    }
};

int main() {
    HashMap mymap(4);   // chota size rakha taaki resize jaldi trigger ho

    mymap.insert(10, 20);
    mymap.insert(30, 40);
    mymap.insert(55, 15);
    mymap.insert(99, 100);   // ye insert resize trigger karega

    bool found;

    int v1 = mymap.get(10, found);
    if (found) cout << "10 -> " << v1 << endl;

    int v2 = mymap.get(30, found);
    if (found) cout << "30 -> " << v2 << endl;

    cout << "bucket count after resize: " << mymap.getBucketCount() << endl;

    mymap.remove(10);
    cout << "delete 48 (not present): " << (mymap.remove(48) ? "true" : "false") << endl;

    mymap.get(10, found);
    if (!found) cout << "10 not found after delete" << endl;

    cout << "size: " << mymap.size() << endl;

    return 0;
}

*/

