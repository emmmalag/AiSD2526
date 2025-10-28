#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

long long int assignments;
long long int comparisons;

void three_way_merge(int arr[], int left, int mid1, int mid2, int right){

    int n1 = mid1 - left + 1;
    int n2 = mid2 - mid1;
    int n3 = right - mid2;

    //temporary subarray (vector because of the dynamic size) for each third
    vector <int> L(n1), M(n2), R(n3);

    for (int i=0; i<n1; i++){
        comparisons++;
        
        L[i] = arr[left + i];
        assignments++;
    }

    for (int i=0; i<n2; i++){
        comparisons++;
        
        M[i] = arr[mid1 + 1 + i];
        assignments++;
    }

    for (int i=0; i<n3; i++){
        comparisons++;
    
        R[i] = arr[mid2 + 1 + i];
        assignments++;
    }

    int i = 0, j = 0, k = 0, index = left;

    //finding and placing the smallest from the three subarrays
    while (i < n1 || j < n2 || k < n3){
        comparisons = comparisons + 3;
        
        int smallest = INT_MAX, smallest_index = -1;
        
        //assigning indexes 0, 1, 2 to subarrays L, M, R respectively
        if (i < n1 && L[i] < smallest){
            comparisons = comparisons + 2;
            
            smallest = L[i];
            smallest_index = 0;
        }
        if (j < n2 && M[j] < smallest){
            comparisons = comparisons + 2;

            smallest = M[j];
            smallest_index = 1;
        }
        if (k < n3 && R[k] < smallest){
            comparisons = comparisons + 2;
    
            smallest = R[k];
            smallest_index = 2;
        }

        //placing it according to index
        if (smallest_index == 0){
            comparisons++;
            
            arr[index++] = L[i++];
            assignments++;
        }
        else if (smallest_index == 1){
            comparisons++;
            
            arr[index++] = M[j++];
            assignments++;
        }
        else{
            arr[index++] = R[k++];
            assignments++;
        }
    }


}

void three_way_merge_sort(int arr[], int left, int right){
    //single element case
    if (right - left < 1) {
        comparisons++;
        return;
    }
    
    //midpoint on the 1/3 of array length
    int mid1 = left + (right - left)/3;
    //same but with 2*1/3
    int mid2 = left + 2*(right-left)/3;

    three_way_merge_sort(arr, left, mid1);
    three_way_merge_sort(arr, mid1 + 1, mid2);
    three_way_merge_sort(arr, mid2+1, right);

    three_way_merge(arr, left, mid1, mid2, right);
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
            three_way_merge_sort(arr, 0, n-1);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }

    return 0;
}