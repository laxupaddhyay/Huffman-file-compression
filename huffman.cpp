#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// A Huffman tree node
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char character, int frequency) {
        ch = character;
        freq = frequency;
        left = right = nullptr;
    }
};

// Compare function for the priority queue
struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// Traverse the Huffman Tree and store the Huffman codes in a map
void generateCodes(struct Node* root, string str, unordered_map<char, string> &huffmanCode) {
    if (root == nullptr)
        return;
    
    // Found a leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

// Build the Huffman Tree and generate the codes
void buildHuffmanTree(string text, unordered_map<char, string> &huffmanCode) {
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    // Create a priority queue to store the live nodes of the Huffman tree
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        Node *left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        // Create a new internal node with frequency equal to the sum of the two nodes' frequencies.
        int sum = left->freq + right->freq;
        Node* node = new Node('\0', sum);

        node->left = left;
        node->right = right;

        pq.push(node);
    }

    // Root stores pointer to the root of the Huffman tree
    Node* root = pq.top();
    generateCodes(root, "", huffmanCode);
}

// Compress the input file using Huffman encoding
void compressFile(string inputFile, string outputFile) {
    ifstream input(inputFile, ios::in);
    if (!input.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    string text((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
    input.close();

    unordered_map<char, string> huffmanCode;
    buildHuffmanTree(text, huffmanCode);

    ofstream output(outputFile, ios::binary);
    if (!output.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Write the Huffman code to the output file
    string encodedStr = "";
    for (char ch : text) {
        encodedStr += huffmanCode[ch];
    }

    // Save encoded data to output file as binary
    output << encodedStr;
    output.close();
}

// Main function
int main() {
    string inputFile = "input.txt";
    string outputFile = "compressed.bin";

    compressFile(inputFile, outputFile);

    cout << "File compressed successfully!" << endl;
    return 0;
}
