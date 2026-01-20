#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

struct Node {
    char symbol;
    int freq;
    vector<Node*> children;
    
    Node(char s, int f) : symbol(s), freq(f) {}
    Node(vector<Node*> kids) : symbol('\0'), freq(0), children(kids) {
        for (Node* child : kids) {
            freq += child->freq;
        }
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* build_ternary_huffmann(unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    
    //dodaj wszystkie symbole jako liście
    for (auto& p : freq) {
        pq.push(new Node(p.first, p.second));
    }
    
    //jeśli liczba węzłów nie pozwala na pełne drzewo ternarne, dodajemy puste węzły o zerowej częstotliwości)
    while (pq.size() > 1 && (pq.size() - 1) % 2 != 0) {
        pq.push(new Node('\0', 0)); // pusty węzeł
    }
    
    //buduj drzewo ternarne
    while (pq.size() > 1) {
        vector<Node*> children;
        for (int i = 0; i < 3 && !pq.empty(); i++) {
            children.push_back(pq.top());
            pq.pop();
        }
        pq.push(new Node(children));
    }
    
    return pq.top();
}

void get_ternary_code(Node* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;
    
    //jeśli to liść z symbolem
    if (root->symbol != '\0' && root->children.empty()) {
        codes[root->symbol] = code;
        return;
    }
    
    //rekurencyjnie przechodzi dzieci
    for (size_t i = 0; i < root->children.size(); i++) {
        get_ternary_code(root->children[i], code + to_string(i), codes);
    }
}

void delete_tree(Node* root) {
    if (!root) return;
    for (Node* child : root->children) {
        delete_tree(child);
    }
    delete root;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> freq_dist(1, 100);
    std::uniform_int_distribution<int> symbol_dist(65, 90); // A-Z

    ofstream output_file("ternary_huffman_results.txt");
    
    for (int k = 0; k < 5; k++) {
        cout << "test: " << k+1 << endl;
        
        for (int n : {500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000, 5500000, 6000000, 6500000, 7000000, 7500000, 8000000, 8500000, 9000000, 9500000, 10000000}) {
            cout << "number of symbols: " << n << endl;
            
            //generuj mapę częstotliwości
            unordered_map<char, int> freq;
            for (int i = 0; i < n; i++) {
                char symbol = static_cast<char>(symbol_dist(gen));
                int frequency = freq_dist(gen);
                freq[symbol] = frequency;
            }
            
            double build_time = 0;
            double codes_time = 0;
            double total_time = 0;
            
            auto start_build = chrono::high_resolution_clock::now();
            Node* root = build_ternary_huffmann(freq);
            auto end_build = chrono::high_resolution_clock::now();
            build_time = chrono::duration_cast<chrono::microseconds>(end_build - start_build).count() / 1000.0;
            
            auto start_codes = chrono::high_resolution_clock::now();
            unordered_map<char, string> codes;
            get_ternary_code(root, "", codes);
            auto end_codes = chrono::high_resolution_clock::now();
            codes_time = chrono::duration_cast<chrono::microseconds>(end_codes - start_codes).count() / 1000.0;
            
            total_time = build_time + codes_time;
 
            cout << "total time: " << total_time << " ms" << endl;
            
            delete_tree(root);
            
            //zapisz do pliku
            output_file << k << ";" << n << ";" 
                       << total_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}