#include <iostream>
#include <vector>
#include <limits>
#include <opencv2/opencv.hpp>

// Przykładowe wartości dla testu
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

struct QuantizationCoefficients {
    std::vector<std::vector<int>> coefficients;
};

int calculateSAD(const std::vector<std::vector<int>>& block1, const std::vector<std::vector<int>>& block2) {
    int sum = 0;
    for (size_t i = 0; i < block1.size(); ++i) {
        for (size_t j = 0; j < block1[i].size(); ++j) {
            sum += std::abs(block1[i][j] - block2[i][j]);
        }
    }
    return sum;
}

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

// Funkcja do przewidywania bloku dla trybu DC
std::vector<std::vector<int>> predictDC(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Oblicz średnią wartość pikseli w bloku
    int sum = 0;
    for (size_t i = 0; i < block.pixels.size(); ++i) {
        for (size_t j = 0; j < block.pixels[i].size(); ++j) {
            sum += block.pixels[i][j];
        }
    }
    int average = sum / (BlockSize * BlockSize);

    // Ustaw przewidywane piksele na średnią wartość
    for (size_t i = 0; i < predictedPixels.size(); ++i) {
        for (size_t j = 0; j < predictedPixels[i].size(); ++j) {
            predictedPixels[i][j] = average;
        }
    }

    return predictedPixels;
}

// Funkcja do przewidywania bloku dla trybu planarnego
std::vector<std::vector<int>> predictPlanar(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Oblicz średnią wartość pikseli w pierwszym wierszu i pierwszej kolumnie bloku
    int sumRow = 0;
    int sumCol = 0;
    for (size_t i = 0; i < block.pixels.size(); ++i) {
        sumRow += block.pixels[0][i];
        sumCol += block.pixels[i][0];
    }
    int averageRow = sumRow / BlockSize;
    int averageCol = sumCol / BlockSize;

    // Wykonaj przewidywanie planarne dla pozostałych pikseli bloku
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

// Funkcja do przewidywania bloku dla trybu kątowego
std::vector<std::vector<int>> predictAngular(const Block& block) {
    std::vector<std::vector<int>> predictedPixels(BlockSize, std::vector<int>(BlockSize, 0));

    // Przykładowe wartości kątów dla przewidywania
    std::vector<int> angles = { 0, 1, 2, 3 }; // Można dostosować wartości kątów do wymagań

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
PredictedBlock analyzeBlock(const Block& block) {
    PredictedBlock predictedBlock;
    int minSAD = std::numeric_limits<int>::max(); // Inicjalizacja z dużą wartością
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

        // Oblicz błąd przewidywania SAD
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

int main() {
    cv::Mat image = cv::imread("lenna.png", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cout << "Failed to load image." << std::endl;
        return 1;
    }

    int startX = 0; // Współrzędna x początku bloku
    int startY = 0; // Współrzędna y początku bloku

       

    Block block = readBlockFromImage(image, startX, startY);
    PredictedBlock predictedBlock = analyzeBlock(block);

    std::cout << "Predicted mode: " << predictedBlock.mode << std::endl;
    std::cout << "Predicted pixels:" << std::endl;
    for (const auto& row : predictedBlock.pixels) {
        for (int pixel : row) {
            std::cout << pixel << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}