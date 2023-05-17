#include <iostream>
#include <vector>
#include <limits>
#include <opencv2/opencv.hpp>
#include "BlockPrediction.h"
#include "BlockTransformsQuantization.h"

// Funkcja do wczytywania bloku pikseli z obrazu
Block readBlockFromImage(const cv::Mat& image, int startX, int startY) {
    Block block;
    block.pixels.resize(BlockSize, std::vector<int>(BlockSize, 0));

    for (int i = 0; i < BlockSize; ++i) {
        for (int j = 0; j < BlockSize; ++j) {
            int pixelX = startX + j;
            int pixelY = startY + i;
            block.pixels[i][j] = static_cast<int>(image.at<uchar>(pixelY, pixelX));
        }
    }

    return block;
}

int main() {
    BlockPrediction bp;
    BlockTransformsQuantization btq;
    cv::Mat image = cv::imread("lenna.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Failed to load image." << std::endl;
        return 1;
    }

    int startX = 0;  // Współrzędna x początku bloku
    int startY = 0;  // Współrzędna y początku bloku

    Block block = readBlockFromImage(image, startX, startY);
    PredictedBlock predictedBlock = bp.analyzeBlock(block);
    TransformedBlock transformedBlock;
    transformedBlock.mode = predictedBlock.mode;
    transformedBlock.coefficients = btq.transformBlock(predictedBlock.pixels);
    std::vector<std::vector<int>> quantizedBlock = btq.quantizeBlock(transformedBlock.coefficients);

    std::cout << "Predicted mode: " << transformedBlock.mode << std::endl;
    std::cout << "Quantized block:" << std::endl;
    for (const auto& row : quantizedBlock) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}