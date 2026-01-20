#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

string LCS_iterative(string X, string Y) {
    int m = X.length();
    int n = Y.length();
    
    vector<vector<int>> c(m+1, vector<int>(n+1, 0));
    vector<vector<char>> b(m+1, vector<char>(n+1, ' '));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i-1] == Y[j-1]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = 'd';  // diagonal
            }
            else if (c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = 'u';  // up
            }
            else {
                c[i][j] = c[i][j-1];
                b[i][j] = 'l';  // left
            }
        }
    }
    
    // Odtwarzanie LCS z macierzy b
    string lcs_result = "";
    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (b[i][j] == 'd') {
            lcs_result = X[i-1] + lcs_result;
            i--;
            j--;
        } else if (b[i][j] == 'u') {
            i--;
        } else {
            j--;
        }
    }
    
    return lcs_result;
}

vector<vector<int>> memo;
string X_global, Y_global;

int LCS_recursive_helper(int i, int j) {
    if (i == 0 || j == 0) return 0;
    
    if (memo[i][j] != -1) return memo[i][j];
    
    if (X_global[i-1] == Y_global[j-1]) {
        memo[i][j] = 1 + LCS_recursive_helper(i-1, j-1);
    } else {
        memo[i][j] = max(LCS_recursive_helper(i-1, j), 
                        LCS_recursive_helper(i, j-1));
    }
    
    return memo[i][j];
}

string get_LCS_from_memo(int i, int j) {
    if (i == 0 || j == 0) return "";
    
    if (X_global[i-1] == Y_global[j-1]) {
        return get_LCS_from_memo(i-1, j-1) + X_global[i-1];
    } else if (memo[i-1][j] >= memo[i][j-1]) {
        return get_LCS_from_memo(i-1, j);
    } else {
        return get_LCS_from_memo(i, j-1);
    }
}

string LCS_recursive_memoized(string X, string Y) {
    int m = X.length();
    int n = Y.length();
    
    X_global = X;
    Y_global = Y;
    
    // Inicjalizacja tablicy spamiętywania
    memo.assign(m+1, vector<int>(n+1, -1));
    
    // Obliczanie długości LCS
    int length = LCS_recursive_helper(m, n);
    
    // Odtwarzanie LCS
    return get_LCS_from_memo(m, n);
}



int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 9);

    ofstream output_file("lcs_results.txt");
    output_file << "test;length;iterative_time;recursive_time\n";
    
    for (int k = 0; k < 5; k++) {
        cout << "test: " << k+1 << endl;
        
        for (int n : {500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500, 9000, 9500, 10000}) {
            cout << "length: " << n << endl;
            
            //dwa przykładowe ciągi
            string X = "";
            string Y = "";
            for (int i = 0; i < n; i++) {
                X += '0' + dist(gen);
                Y += '0' + dist(gen);
            }
            
            double iterative_time = 0;
            double recursive_time = 0;
            
            string lcs_iterative, lcs_recursive;
            int lcs_length = 0;
            bool lcs_match = true;
            
            // test wersji iteracyjnej
            auto start = chrono::high_resolution_clock::now();
            lcs_iterative = LCS_iterative(X, Y);
            auto end = chrono::high_resolution_clock::now();
            iterative_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "iterative: " << iterative_time << endl;
            
            // test wersji rekurencyjnej z spamiętywaniem
            start = chrono::high_resolution_clock::now();
            lcs_recursive = LCS_recursive_memoized(X, Y);
            end = chrono::high_resolution_clock::now();
            recursive_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "recursive: " << recursive_time << endl;
            
            lcs_length = lcs_iterative.length();
            lcs_match = (lcs_iterative == lcs_recursive);
            
            //zapisz do pliku
            output_file << k << ";" << n << ";" 
                       << iterative_time << ";" 
                       << recursive_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}