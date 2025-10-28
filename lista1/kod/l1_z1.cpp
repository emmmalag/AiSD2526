#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

long long int assignments;
long long int comparisons;

void insertion_sort(int arr[], long int n){
    int key;
    int j;
    
    for (int i = 1; i < n; i++){
        comparisons++;
        
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key){
            comparisons = comparisons + 2;
            
            arr[j+1] = arr[j];
            assignments++;
            
            j = j - 1;
        }
        arr[j+1] = key;
        assignments++;
    }
}

int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (long int n: {50000, 100000, 150000, 200000, 250000, 300000, 350000, 400000, 450000, 500000, 550000, 600000, 650000, 700000, 750000, 800000, 850000, 900000, 950000, 1000000}){
        
            assignments = 0;
            comparisons = 0;

            //create and fill array
            //dynamc array to prevent overflow
            int* arr = new int[n];
            for (int i = 0; i < n; i++){
                arr[i] = dist(gen);
            }

            //time sort and output assignments and comparisons
            auto start = std::chrono::high_resolution_clock::now();
            insertion_sort(arr, n);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }

    

    return 0;
}
