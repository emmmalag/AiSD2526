#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

void LCS(string X, string Y) {
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
    
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 9);

    ofstream output_file("lcs_results.txt");
    
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
            
            double lcs_time = 0;
            
            //czas wykonania
            auto start = chrono::high_resolution_clock::now();
            LCS(X, Y);
            auto end = chrono::high_resolution_clock::now();
            lcs_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            
            cout << "LCS time: " << lcs_time << " ms" << endl;
            
            //zapisz do pliku
            output_file << k << ";" << n << ";" << lcs_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}