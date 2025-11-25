#include <iostream>
#include <random>
#include <chrono>
using namespace std;

long long int assignments;
long long int comparisons;

int getMax(int arr[], int n){
    int i;
    int max = arr[0];

    for (int i = 1; i < n; i++){
        if (arr[i] > max){
            comparisons++;
            max = arr[i];
        }
    }
    return max;
}

void counting_sort(int arr[], int n, int digitplace, int d){
    int* sorted_arr = new int[n];
    int* count = new int[d](); //inicjalizacja zerami

    for (int i = 0; i < n; i++){
        int digit = (arr[i]/digitplace)%d;
        count[digit]++;
        assignments++;
    }
    
    for (int i = 1; i < d; i++){
        count[i] += count[i-1];
        assignments++;
    }

    for (int i = n-1; i >= 0; i--){
        int digit = (arr[i] / digitplace) % d;
        sorted_arr[count[digit] - 1] = arr[i];
        count[digit]--;
        assignments += 2;
    }
        
    for (int i = 0; i < n; i++){
        arr[i] = sorted_arr[i];
        assignments++;
    }

    delete[] sorted_arr;
    delete[] count;

}


void radix_sort(int arr[], int n, int d){
   
    if (n <= 1) return; //base case
    comparisons++;

    int max = getMax(arr, n);

    for (int digitplace = 1; max/digitplace > 0; digitplace *= d){
        counting_sort(arr, n, digitplace, d);
    }

}

int main(){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    // Testowane podstawy
    int bases[] = {2, 4, 8, 10, 16};
    int numBases = sizeof(bases) / sizeof(bases[0]);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (int b = 0; b < numBases; b++){
            
            int base = bases[b];

            cout << "base: " << bases[b] << endl;

            for (long int n: {500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000, 5500000, 6000000, 6500000, 7000000, 7500000, 8000000, 8500000, 9000000, 9500000, 10000000}) {
                
                //create and fill array with numbers valid for the current base
                int* arr = new int[n];
                for (int i = 0; i < n; i++){
                    // Generate number with valid digits for the current base
                    int max_valid = 1;
                    while (max_valid * base <= 1000000) {
                        max_valid *= base;
                    }
                    // Use distribution with limited range to ensure valid digits
                    std::uniform_int_distribution<int> base_dist(1, max_valid - 1);
                    arr[i] = base_dist(gen);
                }

                //time sort and output results
                auto start = std::chrono::high_resolution_clock::now();
                radix_sort(arr, n, base);
                auto end = std::chrono::high_resolution_clock::now();
            
                cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
                cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;

                delete[] arr;
            }
        }
    }

    return 0;
}