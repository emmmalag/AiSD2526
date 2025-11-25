#include <iostream>
#include <vector>
#include <chrono>
#include <random>
using namespace std;

long long int assignments;
long long int comparisons;

float getMax(vector<float>& arr, int n){
    int i;
    float max = arr[0];

    for (int i = 1; i < n; i++){
        if (arr[i] > max){
            comparisons++;
            max = arr[i];
        }
    }
    return max;
}

float getMin(vector<float>& arr, int n){
    int i;
    float min = arr[0];

    for (int i = 1; i < n; i++){
        if (arr[i] < min){
            comparisons++;
            min = arr[i];
        }
    }
    return min;
}

void insertion_sort(vector<float>& bucket){
    int key;
    int j;
    
    for (int i = 1; i < bucket.size(); i++){
        comparisons++;
        
        float key = bucket[i];
        int j = i - 1;
        
        while (j >= 0 && bucket[j] > key){
            comparisons = comparisons + 2;
            
            bucket[j+1] = bucket[j];
            assignments++;
            
            j = j - 1;
        }
        bucket[j+1] = key;
        assignments++;
    }
}

void bucket_sort(vector<float>& arr, int n){
    vector<vector<float>> buckets(n);

    float max = getMax(arr, n);
    float min = getMin(arr, n);
    float range = max - min;

    for (int i = 0; i < n; i++){
        //normalizacja do odcinka [0, 1)
        float normal = (arr[i] - min)/range;
        int bi = normal * (n - 2);
        buckets[bi].push_back(arr[i]);
        assignments++;
    }

    for (int i = 0; i < n; i++){
        if (!buckets[i].empty()){
            comparisons++;
            insertion_sort(buckets[i]);
        }
    }

    int index = 0;

    for (int i = 0; i < n; i++){
        for (int j = 0; j < buckets[i].size(); j++){
            arr[index++] = buckets[i][j];
            assignments++;
        }
    }
}

int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1000000);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (long int n: {250000, 500000, 750000, 1000000, 1250000, 1500000, 1750000, 2000000, 2250000, 2500000, 2750000, 3000000, 3250000, 3500000, 3750000, 4000000, 4250000, 4500000, 4750000, 5000000}){
        
            assignments = 0;
            comparisons = 0;

            vector<float> arr(n);
            for (int i = 0; i < n; i++){
                arr[i] = dist(gen);
            }

            //time sort and output assignments and comparisons
            auto start = std::chrono::high_resolution_clock::now();
            bucket_sort(arr, n);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
    
        }
    }

    return 0;
}