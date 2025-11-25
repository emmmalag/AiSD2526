#include <iostream>
#include <random>
#include <chrono>
using namespace std;

long long int assignments;
long long int comparisons;

class Node {
  	public:
    	int val;
    	struct Node* next;
    	Node(int x) {
        	val = x;
        	next = NULL;
    	}
};


Node* sortedInsert(Node* newnode, Node* sorted) {
    
    if (sorted == NULL || 
        sorted->val >= newnode->val) {
        newnode->next = sorted;
        sorted = newnode;
        assignments += 2;
        comparisons++;
    }
    else {
        Node* curr = sorted;
        assignments++;
        
        while (curr->next != NULL && 
               curr->next->val < newnode->val) {
            curr = curr->next;
            assignments++;
            comparisons += 2;
        }
        comparisons += 2;
        
        newnode->next = curr->next;
        curr->next = newnode;
        assignments += 2;
    }
    
    return sorted;
}

Node* insertionSort(Node* head) {
    
    Node* sorted = NULL;
    Node* curr = head;
    assignments += 2;

    while (curr != NULL) {
        comparisons++; 

        Node* next = curr->next;
        assignments++;

        sorted = sortedInsert(curr, sorted);
        assignments++;
        
        curr = next;
        assignments++;
    }
    comparisons++;
    
    return sorted;
}

void deleteList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

int main() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    for (int k = 1; k <= 5; k++){
        cout << "begin test: " << k << endl;
        for (long int n: {50000, 100000, 150000, 200000, 250000, 300000, 350000, 400000, 450000, 500000, 550000, 600000, 650000, 700000, 750000, 800000, 850000, 900000, 950000, 1000000}){
        
            assignments = 0;
            comparisons = 0;

            //create and fill linked list with random values
            Node *head = NULL, *tail = NULL;
            for (int i = 0; i < n; i++){
                int value = dist(gen);
                Node* newNode = new Node(value);
                
                if (head == NULL) {
                    head = newNode;
                    tail = newNode;
                } else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }

            //time sort and output assignments and comparisons
            auto start = std::chrono::high_resolution_clock::now();
            Node* sorted = insertionSort(head);
            auto end = std::chrono::high_resolution_clock::now();
        
            cout << "length: " << n << endl << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << endl;        
            cout << "assignments: " << assignments << endl << "comparisons: " << comparisons << endl;
            
            deleteList(sorted);
        }
    }

    return 0;
}