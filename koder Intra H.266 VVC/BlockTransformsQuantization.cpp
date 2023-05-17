#include "BlockTransformsQuantization.h"

std::vector<std::vector<double>> BlockTransformsQuantization::transformBlock(const std::vector<std::vector<int>>& block) {
    std::vector<std::vector<double>> coefficients(BlockSize, std::vector<double>(BlockSize, 0.0));

    for (size_t u = 0; u < BlockSize; ++u) {
        for (size_t v = 0; v < BlockSize; ++v) {
            double sum = 0.0;
            for (size_t i = 0; i < BlockSize; ++i) {
                for (size_t j = 0; j < BlockSize; ++j) {
                    double angleU = (u == 0) ? std::sqrt(1.0 / BlockSize) : std::sqrt(2.0 / BlockSize);
                    double angleV = (v == 0) ? std::sqrt(1.0 / BlockSize) : std::sqrt(2.0 / BlockSize);
                    double cosineU = std::cos((2.0 * i + 1.0) * u * M_PI / (2.0 * BlockSize));
                    double cosineV = std::cos((2.0 * j + 1.0) * v * M_PI / (2.0 * BlockSize));
                    sum += block[i][j] * angleU * angleV * cosineU * cosineV;
                }
            }
            coefficients[u][v] = sum;
        }
    }

    return coefficients;
}

// Funkcja do kwantyzacji bloku
std::vector<std::vector<int>> BlockTransformsQuantization::quantizeBlock(const std::vector<std::vector<double>>& coefficients) {
    std::vector<std::vector<int>> quantized(BlockSize, std::vector<int>(BlockSize, 0));

    for (size_t i = 0; i < BlockSize; ++i) {
        for (size_t j = 0; j < BlockSize; ++j) {
            quantized[i][j] = std::round(coefficients[i][j] / QuantizationFactor);
        }
    }

    return quantized;
}