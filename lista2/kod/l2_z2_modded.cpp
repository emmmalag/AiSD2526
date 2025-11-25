#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
using namespace std;

long long int assignments;
long long int comparisons;

int getMaxAbs(int arr[], int n){
    int i;
    int max = abs(arr[0]);

    for (int i = 1; i < n; i++){
        if (abs(arr[i]) > max){
            comparisons++;
            max = abs(arr[i]);
        }
    }
    return max;
}

void counting_sort(int arr[], int n, int digitplace, int d){
    int* sorted_arr = new int[n];
    int* count = new int[d](); //inicjalizacja zerami

    for (int i = 0; i < n; i++){
        int digit = (abs(arr[i])/digitplace)%d;
        count[digit]++;
        assignments++;
    }
    
    for (int i = 1; i < d; i++){
        count[i] += count[i-1];
        assignments++;
    }

    for (int i = n-1; i >= 0; i--){
        int digit = (abs(arr[i]) / digitplace) % d;
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
    if (n <= 1) return; //d case
    comparisons++;

    int negative_count = 0;
    int positive_count = 0; // dla ułatwienia 0 jest w positive count

    for (int i = 0; i < n; i++) {
        if (arr[i] < 0) {
            comparisons++;
            negative_count++;
        } else {
            positive_count++;
        }
    }    

    int *negative = new int[negative_count];
    int *positive = new int[positive_count];
    
    int negative_index = 0;
    int positive_index = 0;

    for (int i = 0; i < n; i++){
        if (arr[i] < 0){
            comparisons++;
            negative[negative_index++] = -arr[i];
            assignments++;
        }
        else {
            positive[positive_index++] = arr[i];
            assignments++;
        }
    }


    if (positive_count > 0) {
        comparisons++;
        int maxpositive = getMaxAbs(positive, positive_count);
        for (int digitplace = 1; maxpositive / digitplace > 0; digitplace *= d) {
            counting_sort(positive, positive_count, digitplace, d);
        }
    }
    
    // Sortowanie liczb ujemnych (wartości bezwzględnych)
    if (negative_count > 0) {
        comparisons++;
        int maxNegative = getMaxAbs(negative, negative_count);
        for (int digitplace = 1; maxNegative / digitplace > 0; digitplace *= d) {
            counting_sort(negative, negative_count, digitplace, d);
        }
        
        // Odwracanie kolejności liczb ujemnych i przywracanie znaku
        for (int i = 0; i < negative_count / 2; i++) {
            int temp = negative[i];
            negative[i] = negative[negative_count - 1 - i];
            negative[negative_count - 1 - i] = temp;
            assignments += 2;
        }
        
        for (int i = 0; i < negative_count; i++) {
            negative[i] = -negative[i];
            assignments++;
        }
    }
    
    // Łączenie wyników
    int index = 0;
    for (int i = 0; i < negative_count; i++) {
        arr[index++] = negative[i];
        assignments++;
    }
    for (int i = 0; i < positive_count; i++) {
        arr[index++] = positive[i];
        assignments++;
    }
    
    delete[] negative;
    delete[] positive;
}

int main(){
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(-1000000, 1000000);

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