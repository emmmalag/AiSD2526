#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;


int naive_cut_rod(int p[], int n){
    
    if (n == 0){
        return 0;
    }

    int q = INT_MIN;

    for (int i = 0; i < n; i++){
        q = max(q, p[i] + naive_cut_rod(p, n-i-1));
    }

    return q;
}

int memorized_cut_rod(int p[], int n, int r[]){
    int q;

    if (r[n] >= 0){
        return r[n];
    }
    
    if (n == 0){
        q = 0;
    }
    else{
        q = INT_MIN;

        for (int i = 0; i < n; i++){
            q = max(q, p[i] + memorized_cut_rod(p, n-i-1, r));
        }
    }

    r[n] = q;

    return q;
}

int bottom_up_cut_rod(int p[], int n){
    int r[n+1];
    int q;
    
    r[0] = 0;
    
    for (int j = 1; j <= n; j++){
        q = INT_MIN;
        for (int i = 0; i < j; i++){
            q = max(q, p[i] + r[j-i-1]);
        }
        r[j] = q;
    }
    
    return r[n];
}

int main (){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 10);

    ofstream naive_file("naive_cut_rod_results.txt");
    ofstream dp_file("dp_cut_rod_results.txt");
    
    naive_file << "test;length;naive_time\n";
    dp_file << "test;length;memo_time;bottom_up_time\n";

    int* prices = new int[2001];

    prices[0] = dist(gen);
    for (int i = 1; i < 2001; i++){
        prices[i] = prices[i-1] + dist(gen);
    }
    
    for (int k=0; k<5; k++){
        cout << "test: " << k+1 << endl;
        
        int r_memo[2002];
        for(int i = 0; i <= 2001; i++) r_memo[i] = INT_MIN;

        // małe n dla naive, bo nie wytrzymuje testów z zakresu 10-200 ani 5-100
        for (int n:{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20}){
            cout << "naive length: " << n << endl;

            double naive_time = 0;
            
            //naive
            auto start = chrono::high_resolution_clock::now();
            naive_cut_rod(prices, n);
            auto end = chrono::high_resolution_clock::now();
            naive_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "naive time: " << naive_time << endl;
            
            //wyniki naive
            naive_file << k << ";" << n << ";" << naive_time << "\n";
        }

        //większe n
        for (int n:{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000}){
            cout << "DP length: " << n << endl;
            
            double memo_time = 0;
            double bottom_up_time = 0;
            
            //memorized
            auto start = chrono::high_resolution_clock::now();
            memorized_cut_rod(prices, n, r_memo);
            auto end = chrono::high_resolution_clock::now();
            memo_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "memorized time: " << memo_time << endl;
            
            //bottom-up
            start = chrono::high_resolution_clock::now();
            bottom_up_cut_rod(prices, n);
            end = chrono::high_resolution_clock::now();
            bottom_up_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "bottom-up time: " << bottom_up_time << endl;

            // wyniki memorized i bottom-up
            dp_file << k << ";" << n << ";" << memo_time << ";" << bottom_up_time << "\n";
        }
    }
    
    naive_file.close();
    dp_file.close();
    
    cout << "Results saved to:" << endl;
    cout << "Naive algorithm: naive_cut_rod_results.txt" << endl;
    cout << "DP algorithms: dp_cut_rod_results.txt" << endl;

    delete[] prices;
    return 0;
}