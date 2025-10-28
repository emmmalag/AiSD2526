#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

long long int heap_size;
long long int assignments;
long long int comparisons;

int left(int i){
    return 3*i + 1;
}

int middle(int i){
    return 3*i + 2;
}

int right(int i){
    return 3*i + 3;
}

void ternary_heapify(int arr[], int i){
    int l = left(i);
    int m = middle(i);
    int r = right(i);

    int largest;
    
    if ((l < heap_size) && (arr[l] > arr[i])){
        comparisons = comparisons + 2;
        
        largest = l;
    }
    else{
        largest = i;
    }

    if ((m < heap_size) && (arr[m] > arr[largest])){
        comparisons = comparisons + 2;
        
        largest = m;
    }

    if ((r < heap_size) && (arr[r] > arr[largest])){
        comparisons = comparisons + 2;
        
        largest = r;
    }

    if (i != largest){
        comparisons++;
        
        int temp = arr[i];
        
        arr[i] = arr[largest];
        arr[largest] = temp;
        assignments = assignments + 2;

        ternary_heapify(arr, largest);
    }
}

void ternary_build_heap(int arr[], int n){
    heap_size = n;
    for (int i = floor((n/3)-1); i >= 0; i--){
        comparisons++;
        ternary_heapify(arr, i);
    }
}

void ternary_heap_sort(int arr[], int n){
    ternary_build_heap(arr, n);
    
    for (int i = n-1; i >= 1; i--){
        comparisons++;
        
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        assignments = assignments + 2;

        heap_size = heap_size - 1;
        ternary_heapify(arr, 0);
    }
}

int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (long int n: {5000000, 10000000, 15000000, 20000000, 25000000, 30000000, 35000000, 40000000, 45000000, 50000000, 55000000, 60000000, 65000000, 70000000, 75000000, 80000000, 85000000, 90000000, 95000000, 100000000}){
        
            assignments = 0;
            comparisons = 0;

            //create and fill array
            int* arr = new int[n];
            for (int i = 0; i < n; i++){
                arr[i] = dist(gen);
            }

            //time sort and output assignments and comparisons
            auto start = std::chrono::high_resolution_clock::now();
            ternary_heap_sort(arr, n);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }
    
    return 0;
}