#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

//recursive
vector<int> RECURSIVE_ACTIVITY_SELECTOR(const vector<int>& s, const vector<int>& f, int k, int n) {
    int m = k + 1;
    
    //znajdź następną aktywność bez kolizji godzin
    while (m <= n && s[m] < f[k]) {
        m++;
    }
    
    if (m <= n) {
        vector<int> result = {m};
        vector<int> rest = RECURSIVE_ACTIVITY_SELECTOR(s, f, m, n);
        result.insert(result.end(), rest.begin(), rest.end());
        return result;
    } else {
        return {};
    }
}

//iterative
vector<int> ACTIVITY_SELECTOR(const vector<int>& s, const vector<int>& f) {
    int n = s.size() - 1;
    vector<int> A = {1};
    int k = 1;
    
    for (int m = 2; m <= n; m++) {
        if (s[m] >= f[k]) {
            A.push_back(m);
            k = m;
        }
    }
    
    return A;
}

int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> time_dist(0, 100);
    std::uniform_int_distribution<int> dur_dist(1, 10);

    ofstream output_file("activity_results.txt");
    
    for (int k = 0; k < 5; k++) {
        cout << "test: " << k+1 << endl;
        
        for (int n : {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 95000, 100000}) {
            cout << "number of activities: " << n << endl;
            
            //indeksowane wektory rozpoczęć i końca zajęć
            vector<int> s(n+1, 0); //start
            vector<int> f(n+1, 0); //koniec
            
            //generator aktywności
            vector<pair<int, int>> activities; //plan godzinowy
            for (int i = 1; i <= n; i++) {
                int start = time_dist(gen);
                int finish = start + dur_dist(gen);
                activities.push_back({finish, start});
            }
            
            //sort względem czasu zakończenia
            sort(activities.begin(), activities.end());
            
            for (int i = 1; i <= n; i++) {
                s[i] = activities[i-1].second; //start
                f[i] = activities[i-1].first;  //koniec
            }
            
            double recursive_time = 0;
            double iterative_time = 0;
            
            //recursive
            auto start = chrono::high_resolution_clock::now();
            RECURSIVE_ACTIVITY_SELECTOR(s, f, 1, n);
            auto end = chrono::high_resolution_clock::now();
            recursive_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "recursive time: " << recursive_time << " ms" << endl;
            
            //iterative
            start = chrono::high_resolution_clock::now();
            ACTIVITY_SELECTOR(s, f);
            end = chrono::high_resolution_clock::now();
            iterative_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "iterative time: " << iterative_time << " ms" << endl;
            
            //zapisz do pliku
            output_file << k << ";" << n << ";" 
                       << recursive_time << ";" 
                       << iterative_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}