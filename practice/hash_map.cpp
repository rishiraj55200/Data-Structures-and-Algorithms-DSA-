#include <bits/stdc++.h>
using namespace std;
// create hash map

class node{
    public:

       int key;
       int val;
       node* next;

       public:

       node(int key , int val){
        this->key = key;
        this->val = val;
        next = NULL;
       }
};

class hash_map{
    private:
       vector<node*>hash;
       int s;
    
    public:
       hash_map(int s){
        this->s = s;
        hash.resize(s , NULL);
       }

        int find_index(int key){

            return (key%s + s)%s;
        }

       void insert(int key , int val){
           int index = find_index(key);
           node* curr  = hash[index];

           while(curr != NULL){
               if(curr->key == key){
                  curr->val = val;
                  return;
               }

               curr = curr->next;
           }

           node* newnode = new node(key , val);
           curr = hash[index];
           newnode->next = curr;
           hash[index] = newnode;

       }

       int get(int key){
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

       void delete_node(int key){
        int index = find_index(key);
        node* curr = hash[index];
        node* prev = NULL;

        while(curr != NULL){
            if(curr->key == key)break;
            prev = curr;
            curr = curr->next;
        }

        if(curr == NULL){
            cout << "node is present in hash_map"<<endl;
            return ;
            
        }
        else{
              if(prev == NULL){
            hash[index] = curr->next;
        }
        else{
            prev->next = curr->next;
        }

        delete curr;
        }

       


       }

       ~hash_map(){
         for(int i = 0; i<s; i++){
            node* curr = hash[i];
            while(curr != NULL){
                node* temp = curr;
                curr = curr->next;
                delete temp;
            }
         }
       }


    
};

int main(){
     hash_map mymap(100);

     mymap.insert( 10 , 20);
     mymap.insert(30 , 40);
     mymap.insert(55 , 15);

    cout << mymap.get(10) << endl;
     cout << mymap.get(30) << endl;

     mymap.delete_node(10);
     mymap.delete_node(48);

      cout << mymap.get(10) << endl;



    return 0;
}
