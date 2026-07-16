#include <bits/stdc++.h>
using namespace std;

int solve(int prev , int i , int m , vector<int>&arr , int n){
     // base case

       int ans  = 0 ;
     if(arr[i] == 0){

        for(int j = 1; j<=m; j++){
             if(prev != -1 && abs(arr[prev] - i)<= 1 && i+){
                
             }
        }
     }



}

int main(){
     int n , x ; 
     cin >> n;
     cin >> x;
     vector<int>arr(n);
     for(int i = 0; i<n; i++){
        cin >> arr[i];
     }

     solve(0 , -1 , x , arr);
    
    return 0;
}