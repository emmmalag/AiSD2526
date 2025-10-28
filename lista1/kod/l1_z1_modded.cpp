#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

long long int assignments;
long long int comparisons;

void modded_insertion_sort(int arr[], int n){
    int key1;
    int key2;
    int j;
    
    if (n <= 1){
        comparisons++;
        return;
    }

    for (int i = 1; i < n; i+=2){
        comparisons++;
        
        if (i+1 >= n){
            comparisons++;
            break;
        }

        int key1 = arr[i];
        int key2 = arr[i+1];

        //key comparison to order ascendingly
        if (key1 > key2){
            comparisons++;
            
            int temp = key1;
            key1 = key2;
            key2 = temp;
        }


        int j = i - 1;
        
        while (j >= 0 && arr[j] > key2){
            comparisons = comparisons + 2;
            
            arr[j+2] = arr[j];
            assignments++;

            j = j - 1;
        }

        arr[j+2] = key2;
        assignments++;

        while(j >= 0 && arr[j] > key1){
            comparisons = comparisons + 2;
            
            arr[j+1] = arr[j];
            assignments++;

            j = j - 1;
        }

        arr[j+1] = key1;
        assignments++;

        if (n % 2 != 0) {
            comparisons++;

            int last = arr[n - 1];
            int j = n - 2;
            
            while (j >= 0 && arr[j] > last) {
                comparisons = comparisons + 2;
                
                arr[j + 1] = arr[j];
                assignments++;
                j--;
            }
            arr[j + 1] = last;
            assignments++;
    }
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
            modded_insertion_sort(arr, n);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
            delete[] arr;
        }
    }

    return 0;
}
