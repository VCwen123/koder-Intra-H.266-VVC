#pragma once
#include <string>
#include <unordered_map>
#include <queue>

// Struktura wêz³a drzewa Huffmana
struct HuffmanNode
{
	int value;
	int frequency;
	HuffmanNode* left;
	HuffmanNode* right;
	HuffmanNode(int val, int freq) : value(val), frequency(freq), left(nullptr), right(nullptr) {}
};

// Funkcja pomocnicza do porównywania wêz³ów Huffmana
struct Compare {
	bool operator()(HuffmanNode* a, HuffmanNode* b) {
		return a->frequency > b->frequency;
	}
};

class HuffmanCoding
{
public:
	// Funkcja do generowania kodów Huffmana
	void generateHuffmanCodes(HuffmanNode* root, std::string code, std::unordered_map<int, std::string>& huffmanCodes);
	// Funkcja do zakodowania danych przy u¿yciu kodowania Huffmana
	std::vector<std::string> encodeUsingHuffman(const std::vector<int>& encodedData);

};

