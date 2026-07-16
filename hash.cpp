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


