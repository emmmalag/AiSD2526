#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <chrono>
#include <random>
#include <fstream>
using namespace std;

struct Node {
    char symbol;
    int freq;
    Node *left, *right;
    
    Node(char s, int f) : symbol(s), freq(f), left(nullptr), right(nullptr) {}
    Node(Node* l, Node* r) : symbol('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node* buildHuffman(unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    
    for (auto& p : freq) {
        pq.push(new Node(p.first, p.second));
    }
    
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(new Node(left, right));
    }
    
    return pq.top();
}

void getCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        codes[root->symbol] = code;
    }
    
    getCodes(root->left, code + "0", codes);
    getCodes(root->right, code + "1", codes);
}

void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> freq_dist(1, 100); //częstotliwości 1-100
    std::uniform_int_distribution<int> symbol_dist(65, 90); //duże litery A-Z (65-90 ASCII)

    ofstream output_file("huffman_results.txt");
    
    for (int k = 0; k < 5; k++) {
        cout << "test: " << k+1 << endl;
        
        for (int n : {500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000, 4500000, 5000000, 5500000, 6000000, 6500000, 7000000, 7500000, 8000000, 8500000, 9000000, 9500000, 10000000}) {
            cout << "number of symbols: " << n << endl;
            
            //generuj mapę częstotliwości dla n symboli
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
            Node* root = buildHuffman(freq);
            auto end_build = chrono::high_resolution_clock::now();
            build_time = chrono::duration_cast<chrono::microseconds>(end_build - start_build).count() / 1000.0;
            
            auto start_codes = chrono::high_resolution_clock::now();
            unordered_map<char, string> codes;
            getCodes(root, "", codes);
            auto end_codes = chrono::high_resolution_clock::now();
            codes_time = chrono::duration_cast<chrono::microseconds>(end_codes - start_codes).count() / 1000.0;
            
            total_time = build_time + codes_time;
            
            cout << "total time: " << total_time << " ms" << endl;
            
            deleteTree(root);
            
            //zapisz do pliku
            output_file << k << ";" << n << ";" 
                       << total_time << "\n";
        }
    }
    
    output_file.close();
    
    return 0;
}