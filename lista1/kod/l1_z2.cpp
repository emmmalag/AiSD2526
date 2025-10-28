#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

long long int assignments;
long long int comparisons;

//first subarray is L[left..mid]
//second subarray is R[mid+1..right]
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Use dynamic allocation instead of stack arrays
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++){
        comparisons++;
        L[i] = arr[left + i];
        assignments++;
    }
    for (int j = 0; j < n2; j++){
        comparisons++;
        R[j] = arr[mid + 1 + j];
        assignments++;
    }

    int i = 0;
    int j = 0;
    int k = left;
    
    //placing the smallest element
    while (i < n1 && j < n2) {
        comparisons = comparisons + 2;
        
        if (L[i] <= R[j]) {
            comparisons++;
            arr[k] = L[i];
            assignments++;
            i++;
        } else {
            arr[k] = R[j];
            assignments++;
            j++;
        }
        k++;
    }

    //copy the remaining elements of L[], if any
    while (i < n1) {
        comparisons++;
        arr[k] = L[i];
        assignments++;
        i++;
        k++;
    }

    //copy the remaining elements of R[], if any
    while (j < n2) {
        comparisons++;
        arr[k] = R[j];
        assignments++;
        j++;
        k++;
    }

    // Clean up dynamically allocated memory
    delete[] L;
    delete[] R;
}

//subarray to be sorted is in the index range [left..right]
void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        comparisons++;

        //calculate the midpoint
        int mid = left + (right - left) / 2;

        //sort first and second halves
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);

        //merge the sorted halves
        merge(arr, left, mid, right);
    }
}

int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (long int n: {2500000, 5000000, 7500000, 10000000, 12500000, 15000000, 17500000, 20000000, 22500000, 25000000, 27500000, 30000000, 32500000, 35000000, 37500000, 40000000, 42500000, 45000000, 47500000, 50000000}){
        
            assignments = 0;
            comparisons = 0;

            //create and fill array
            int* arr = new int[n];
            for (int i = 0; i < n; i++){
                arr[i] = dist(gen);
            }

            //time sort and output assignments and comparisons
            auto start = std::chrono::high_resolution_clock::now();
            merge_sort(arr, 0, n-1);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }

    return 0;
}