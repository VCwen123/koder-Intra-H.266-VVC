#pragma once
#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>
#include "BlockPrediction.h"

const int QuantizationFactor = 10;

struct TransformedBlock {
	IntraMode mode;
	std::vector<std::vector<double>> coefficients;
};

class BlockTransformsQuantization
{
public:
	// Funkcja do przekszta�cania bloku za pomoc� DCT
	std::vector<std::vector<double>> transformBlock(const std::vector<std::vector<int>>& block);
	// Funkcja do kwantyzacji bloku
	std::vector<std::vector<int>> quantizeBlock(const std::vector<std::vector<double>>& coefficients);
	// Funkcja do kwantyzacji wektor�w
	std::vector<int> quantizeVectors(const std::vector<std::string>& encodedData);
};

