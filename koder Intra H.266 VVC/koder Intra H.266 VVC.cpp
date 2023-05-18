#include <iostream>
#include <vector>
#include <limits>
#include <opencv2/opencv.hpp>
#include "BlockPrediction.h"
#include "BlockTransformsQuantization.h"
#include "HuffmanCoding.h"

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
    HuffmanCoding hc;

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

    // Kodowanie różnicowe na macierzy quantizedBlock
    std::vector<std::vector<int>> differentialBlock(BlockSize, std::vector<int>(BlockSize, 0));

    // Pierwszy piksel bez zmian
    differentialBlock[0][0] = quantizedBlock[0][0];

    for (size_t i = 0; i < BlockSize; ++i) {
        for (size_t j = 0; j < BlockSize; ++j) {
            if (i == 0 && j == 0) 
            {
                continue;  // Pomijamy pierwszy piksel, który już został przypisany
            }

            // Obliczanie różnicy między kolejnymi pikselami
            int diff;
            if (i == 0 || j == 0) {
                diff = quantizedBlock[i][j];  // Przypisanie samej wartości piksela, gdy indeks jest równy 0
            }
            else {
                diff = quantizedBlock[i][j] - quantizedBlock[i - 1][j - 1];
            }
            differentialBlock[i][j] = diff;
        }
    }

    // Przekształcenie macierzy differentialBlock na wektor encodedData
    std::vector<int> encodedData;
    encodedData.reserve(BlockSize * BlockSize);  // Rezerwujemy miejsce w wektorze dla efektywności

    for (size_t i = 0; i < BlockSize; ++i) {
        for (size_t j = 0; j < BlockSize; ++j) {
            encodedData.push_back(differentialBlock[i][j]);
        }
    }

    std::vector<std::string> encoded = hc.encodeUsingHuffman(encodedData);

    // Wyświetlanie zakodowanych danych
    for (const std::string& code : encoded) {
        std::cout << code << " ";
    }

    std::vector<int> quantized = btq.quantizeVectors(encoded);

    // Wyświetlanie zakodowanych danych po kwantyzacji
    std::cout << "po kwantyzacji";
    for (int value : quantized) {
        std::cout << value << " ";
    }

    return 0;
}