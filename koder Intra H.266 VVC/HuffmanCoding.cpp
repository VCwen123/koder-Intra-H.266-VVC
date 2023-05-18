#include "HuffmanCoding.h"


void HuffmanCoding::generateHuffmanCodes(HuffmanNode* root, std::string code, std::unordered_map<int, std::string>& huffmanCodes) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        huffmanCodes[root->value] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Funkcja do zakodowania danych przy u¿yciu kodowania Huffmana
std::vector<std::string> HuffmanCoding::encodeUsingHuffman(const std::vector<int>& encodedData) {
    // Obliczanie czêstoœci wyst¹pieñ poszczególnych pikseli
    std::unordered_map<int, int> frequencyMap;
    for (int pixel : encodedData) {
        frequencyMap[pixel]++;
    }

    // Tworzenie wêz³ów Huffmana
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;
    for (const auto& pair : frequencyMap) {
        HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
        pq.push(node);
    }

    // Budowanie drzewa Huffmana
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* combined = new HuffmanNode(-1, left->frequency + right->frequency);
        combined->left = left;
        combined->right = right;

        pq.push(combined);
    }

    // Korzeñ drzewa Huffmana
    HuffmanNode* root = pq.top();

    // Generowanie kodów Huffmana
    std::unordered_map<int, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Zakodowane dane
    std::vector<std::string> encoded;

    // Kodowanie danych na podstawie wygenerowanych kodów Huffmana
    for (int pixel : encodedData) {
        encoded.push_back(huffmanCodes[pixel]);
    }

    return encoded;
}