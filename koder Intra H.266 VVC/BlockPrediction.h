#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

const int BlockSize = 32;
const int NumIntraModes = 3;  // Liczba trybów kodowania Intra

struct Block {
    std::vector<std::vector<int>> pixels;
};

enum IntraMode {
    MODE_DC,
    MODE_PLANAR,
    MODE_ANGULAR,
    //inne tryby kodowania Intra
};

struct PredictedBlock {
    IntraMode mode;
    std::vector<std::vector<int>> pixels;  // Dodaj informacje o pikselach bloku przewidzianego
};

class BlockPrediction
{
public:   
    int calculateSAD(const std::vector<std::vector<int>>& block1, const std::vector<std::vector<int>>& block2);
    // Funkcja do przewidywania bloku dla trybu DC
    std::vector<std::vector<int>> predictDC(const Block& block);
    // Funkcja do przewidywania bloku dla trybu planarnego
    std::vector<std::vector<int>> predictPlanar(const Block& block);
    // Funkcja do przewidywania bloku dla trybu k¹towego
    std::vector<std::vector<int>> predictAngular(const Block& block);
    // Funkcja do analizy bloku obrazu
    PredictedBlock analyzeBlock(const Block& block);
};

