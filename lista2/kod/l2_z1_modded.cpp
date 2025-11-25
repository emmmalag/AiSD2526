#include <iostream>
#include <random>
#include <chrono>
using namespace std;

long long int assignments;
long long int comparisons;

void three_way_partition(int arr[], int p, int k, int& i, int& j){
    int x = arr[k];
    int low = p;
    int mid = p;
    int high = k;

    while (mid <= high){
        if (arr[mid] < x){
            comparisons++;
            swap(arr[low], arr[mid]);
            assignments += 2;
            low++;
            mid++;
        }
        else if (arr[mid] == x){
            comparisons++;
            mid++;
        }
        else{
            comparisons++;
            swap(arr[mid], arr[high]);
            assignments += 2;
            high--;
        }
    }

    i = low -1;
    j = mid;
}

void quick_sort(int arr[], int p, int k){
    if (p < k){
        comparisons++;
        
        int i, j;
        three_way_partition(arr, p, k, i, j);

        quick_sort(arr, p, i);
        quick_sort(arr, j, k);
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
            quick_sort(arr, 0, n);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }

    return 0;
}