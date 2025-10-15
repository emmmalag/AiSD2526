#include <iostream>
using namespace std;

void insertion_sort(int arr[], int n){
    int key;
    int j;
    
    for (int i = 2; i < n; i++){
        int key = arr[i];
        int j = i - 1;
        
        while (j > 0 && arr[j] > key){
            arr[j+1] = arr[j];
            j = j - 1;
        }
        arr[j+1] = key;
    }
}

void modded_insertion_sort(int arr[], int n){

//zrob to w weekend idk

}

int main(){



    return 0;
}
