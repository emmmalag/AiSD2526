#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

struct Activity {
    int start;
    int finish;
    int index; // oryginalny indeks zajęć
};

//rekurencyjny
vector<int> RECURSIVE_ACTIVITY_SELECTOR_SORTED_BY_START(const vector<Activity>& activities, int k) {
    int n = activities.size();
    int m = k + 1;
    
    //szukamy pierwszego zajęcia które zaczyna się po zakończeniu k-tego zajęcia
    while (m < n && activities[m].start < activities[k].finish) {
        m++;
    }
    
    if (m < n) {
        vector<int> result = {activities[m].index};
        vector<int> rest = RECURSIVE_ACTIVITY_SELECTOR_SORTED_BY_START(activities, m);
        result.insert(result.end(), rest.begin(), rest.end());
        return result;
    }
    
    return {};
}

//pomocnicza do wywołania rekurencyjnego
vector<int> RECURSIVE_ACTIVITY_SELECTOR_MODIFIED(const vector<Activity>& activities) {
    int n = activities.size();
    if (n == 0) return {};
    
    //najczybciej się kończą
    int earliest_finish_index = 0;
    for (int i = 1; i < n; i++) {
        if (activities[i].finish < activities[earliest_finish_index].finish) {
            earliest_finish_index = i;
        }
    }
    
    //pierwszy wybór
    vector<int> result = {activities[earliest_finish_index].index};
    
    // szukamy kolejnych zajęć
    int current_finish = activities[earliest_finish_index].finish;
    for (int i = earliest_finish_index + 1; i < n; i++) {
        if (activities[i].start >= current_finish) {
            result.push_back(activities[i].index);
            current_finish = activities[i].finish;
        }
    }
    
    return result;
}

//iteracyjny
vector<int> ITERATIVE_ACTIVITY_SELECTOR_MODIFIED(const vector<Activity>& activities) {
    int n = activities.size();
    if (n == 0) return {};
    
    //musimy znaleźć zajęcie które kończy się najwcześniej
    int earliest_finish_index = 0;
    for (int i = 1; i < n; i++) {
        if (activities[i].finish < activities[earliest_finish_index].finish) {
            earliest_finish_index = i;
        }
    }
    
    vector<int> A = {activities[earliest_finish_index].index};
    int last_finish = activities[earliest_finish_index].finish;
    
    //pozostałe zajęcia (już posortowane wg start)
    for (int i = earliest_finish_index + 1; i < n; i++) {
        if (activities[i].start >= last_finish) {
            A.push_back(activities[i].index);
            last_finish = activities[i].finish;
        }
    }
    
    return A;
}

//dynamiczne
vector<int> DP_ACTIVITY_SELECTOR(const vector<Activity>& activities) {
    int n = activities.size();
    if (n == 0) return {};
    
    //kopia z sortowaniem wg czasu rozpoczęcia
    vector<Activity> sorted_activities = activities;
    sort(sorted_activities.begin(), sorted_activities.end(), 
         [](const Activity& a, const Activity& b) {
             return a.start < b.start;
         });
    
    // dp[i] - maksymalna liczba zajęć które można wybrać,
    // kończących się na i-tym zajęciu (lub wcześniej)
    vector<int> dp(n, 1);
    vector<int> prev(n, -1);
    
    //algorytm dynamiczny
    for (int i = 1; i < n; i++) {
        dp[i] = 1;
        prev[i] = -1;
        
        for (int j = 0; j < i; j++) {
            if (sorted_activities[j].finish <= sorted_activities[i].start) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                    prev[i] = j;
                }
            }
        }
    }
    
    //znajdź max
    int max_index = 0;
    for (int i = 1; i < n; i++) {
        if (dp[i] > dp[max_index]) {
            max_index = i;
        }
    }
    
    //odtwórz rozwiązanie
    vector<int> result;
    int current = max_index;
    while (current != -1) {
        result.push_back(sorted_activities[current].index);
        current = prev[current];
    }
    
    reverse(result.begin(), result.end());
    return result;
}

//pomocnicze do wyświetlania
void printActivities(const vector<Activity>& activities, const string& title) {
    cout << title << ":" << endl;
    for (const auto& act : activities) {
        cout << "Zajęcie " << act.index << ": [" 
             << act.start << " - " << act.finish << "]" << endl;
    }
    cout << endl;
}

void printResult(const vector<int>& result, const string& title) {
    cout << title << ": ";
    if (result.empty()) {
        cout << "brak zajęć";
    } else {
        for (size_t i = 0; i < result.size(); i++) {
            cout << result[i];
            if (i < result.size() - 1) cout << " ";
        }
    }
    cout << " (liczba: " << result.size() << ")" << endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> time_dist(0, 100); // czas 0-100
    std::uniform_int_distribution<int> dur_dist(1, 10);   // długość zajęć 1-10

    ofstream output_file("activity_results_modded.txt");
    
    for (int k = 0; k < 5; k++) {
        cout << "test: " << k+1 << endl;
        
        for (int n : {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 95000, 100000}) {
            cout << "number of activities: " << n << endl;
            
            // Generuj n losowych zajęć
            vector<Activity> activities;
            for (int i = 0; i < n; i++) {
                int start = time_dist(gen);
                int finish = start + dur_dist(gen);
                activities.push_back({start, finish, i});
            }
            
            // Sortuj według czasu rozpoczęcia dla rekurencyjnego
            vector<Activity> sorted_activities = activities;
            sort(sorted_activities.begin(), sorted_activities.end(), 
                [](const Activity& a, const Activity& b) {
                    return a.start < b.start;
                });
            
            double recursive_time = 0;
            double iterative_time = 0;
            double dp_time = 0;
            
            // Rekurencyjny
            auto start = chrono::high_resolution_clock::now();
            RECURSIVE_ACTIVITY_SELECTOR_MODIFIED(activities);
            auto end = chrono::high_resolution_clock::now();
            recursive_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "recursive time: " << recursive_time << " ms" << endl;
            
            // Iteracyjny
            start = chrono::high_resolution_clock::now();
            ITERATIVE_ACTIVITY_SELECTOR_MODIFIED(activities);
            end = chrono::high_resolution_clock::now();
            iterative_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "iterative time: " << iterative_time << " ms" << endl;
            
            // Dynamiczny
            start = chrono::high_resolution_clock::now();
            DP_ACTIVITY_SELECTOR(activities);
            end = chrono::high_resolution_clock::now();
            dp_time = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;
            cout << "DP time: " << dp_time << " ms" << endl;
            
            // Zapisz do pliku
            output_file << k << ";" << n << ";" 
                       << recursive_time << ";" 
                       << iterative_time << ";" 
                       << dp_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}