#include "BlockPrediction.h"

int BlockPrediction::calculateSAD(const std::vector<std::vector<int>>& block1, const std::vector<std::vector<int>>& block2)
{
    int sum = 0;
    for (size_t i = 0; i < block1.size(); ++i) {
        for (size_t j = 0; j < block1[i].size(); ++j) {
            sum += std::abs(block1[i][j] - block2[i][j]);
        }
    }
    return sum;
}

// Funkcja do przewidywania bloku dla trybu DC
std::vector<std::vector<int>> BlockPrediction::predictDC(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Oblicz œredni¹ wartoœæ pikseli w bloku
    int sum = 0;
    for (size_t i = 0; i < block.pixels.size(); ++i) {
        for (size_t j = 0; j < block.pixels[i].size(); ++j) {
            sum += block.pixels[i][j];
        }
    }
    int average = sum / (BlockSize * BlockSize);

    // Ustaw przewidywane piksele na œredni¹ wartoœæ
    for (size_t i = 0; i < predictedPixels.size(); ++i) {
        for (size_t j = 0; j < predictedPixels[i].size(); ++j) {
            predictedPixels[i][j] = average;
        }
    }

    return predictedPixels;
}

// Funkcja do przewidywania bloku dla trybu planarnego
std::vector<std::vector<int>> BlockPrediction::predictPlanar(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Oblicz œredni¹ wartoœæ pikseli w pierwszym wierszu i pierwszej kolumnie bloku
    int sumRow = 0;
    int sumCol = 0;
    for (size_t i = 0; i < block.pixels.size(); ++i) {
        sumRow += block.pixels[0][i];
        sumCol += block.pixels[i][0];
    }
    int averageRow = sumRow / BlockSize;
    int averageCol = sumCol / BlockSize;

    // Wykonaj przewidywanie planarne dla pozosta³ych pikseli bloku
    for (size_t i = 0; i < predictedPixels.size(); ++i) {
        predictedPixels[i][0] = averageCol + i;
        predictedPixels[0][i] = averageRow + i;
    }
    for (size_t i = 1; i < predictedPixels.size(); ++i) {
        for (size_t j = 1; j < predictedPixels[i].size(); ++j) {
            predictedPixels[i][j] = predictedPixels[i][0] + predictedPixels[0][j] - averageRow;
        }
    }

    return predictedPixels;
}

// Funkcja do przewidywania bloku dla trybu k¹towego
std::vector<std::vector<int>> BlockPrediction::predictAngular(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Przyk³adowe wartoœci k¹tów dla przewidywania
    std::vector<int> angles = { 0, 1, 2, 3 }; // Mo¿na dostosowaæ wartoœci k¹tów do wymagañ

    for (size_t i = 0; i < BlockSize; ++i) {
        for (size_t j = 0; j < BlockSize; ++j) {
            int x = static_cast<int>(i);
            int y = static_cast<int>(j);
            int a = angles[(x + y) % angles.size()];
            int b = (x + y) / angles.size();

            int topLeft = (b + 1) * block.pixels[0][0];
            int top = (BlockSize - 1 - x) * block.pixels[0][j];
            int left = (BlockSize - 1 - y) * block.pixels[i][0];

            predictedPixels[i][j] = (topLeft + top + left) / (BlockSize - 1);
        }
    }

    return predictedPixels;
}

// Funkcja do analizy bloku obrazu
PredictedBlock BlockPrediction::analyzeBlock(const Block& block) {
    PredictedBlock predictedBlock;
    int minSAD = std::numeric_limits<int>::max(); // Inicjalizacja z du¿¹ wartoœci¹
    for (int mode = 0; mode < NumIntraModes; ++mode) {
        // Wykonaj przewidywanie bloku dla danego trybu kodowania Intra
        std::vector<std::vector<int>> predictedPixels;
        if (mode == MODE_DC) {
            predictedPixels = predictDC(block);
        }
        else if (mode == MODE_PLANAR) {
            predictedPixels = predictPlanar(block);
        }
        else if (mode == MODE_ANGULAR) {
            predictedPixels = predictAngular(block);
        }

        // Oblicz b³¹d przewidywania SAD
        int currentSAD = calculateSAD(block.pixels, predictedPixels);
        std::cout << "SAD: " << currentSAD << std::endl;

        if (currentSAD < minSAD) {
            minSAD = currentSAD;
            predictedBlock.mode = static_cast<IntraMode>(mode);
            predictedBlock.pixels = predictedPixels;
        }
    }
    return predictedBlock;
}