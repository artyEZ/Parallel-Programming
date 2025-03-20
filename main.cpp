#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <filesystem>
#include <sstream>

const std::string SAVE_PATH = "C:/Users/artyo/Desktop/Учеба/Параллельное программирование/Lab1";

// Функция для создания квадратной матрицы
std::vector<std::vector<int>> createSquareMatrix(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 99);

    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = distrib(gen);
        }
    }
    return matrix;
}

// Функция для записи матрицы в файл
void writeMatrixToFile(const std::vector<std::vector<int>>& matrix, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filepath << std::endl;
        exit(1);
    }

    int size = matrix.size();
    file << size << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            file << matrix[i][j] << " ";
        }
        file << std::endl;
    }
    file.close();
}

// Функция для умножения матриц
std::vector<std::vector<int>> multiplyMatrices(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    if (colsA != rowsB) {
        std::cerr << "Error: Matrices cannot be multiplied. Columns of A must match rows of B." << std::endl;
        exit(1);
    }

    std::vector<std::vector<int>> result(rowsA, std::vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

int main() {
    std::vector<int> sizes = { 10, 50, 100, 200, 500, 1000 }; // Размеры матриц
    int num_runs = 5; // Количество запусков для усреднения времени

    if (!std::filesystem::exists(SAVE_PATH)) {
        std::cerr << "Error: The specified directory does not exist." << std::endl;
        return 1;
    }

    std::ofstream resultsFile(SAVE_PATH + "/results.csv");
    resultsFile << "Size,C++ Time (seconds),Python Time (seconds)\n";

    for (int size : sizes) {
        double total_time = 0.0;

        for (int run = 0; run < num_runs; ++run) {
            std::vector<std::vector<int>> matrixA = createSquareMatrix(size);
            std::vector<std::vector<int>> matrixB = createSquareMatrix(size);

            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<int>> result = multiplyMatrices(matrixA, matrixB);
            auto end = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration<double>(end - start).count();

            if (run == 0) { // Сохраняем матрицы только при первом запуске
                std::ostringstream oss;
                oss << SAVE_PATH << "/matrixA_size_" << size << ".txt";
                writeMatrixToFile(matrixA, oss.str());

                oss.str("");
                oss << SAVE_PATH << "/matrixB_size_" << size << ".txt";
                writeMatrixToFile(matrixB, oss.str());

                oss.str("");
                oss << SAVE_PATH << "/result_size_" << size << ".txt";
                writeMatrixToFile(result, oss.str());
            }
        }

        double average_time = total_time / num_runs;
        resultsFile << size << "," << average_time << ",\n"; // Время Python будет добавлено позже
        std::cout << "Matrix size: " << size << "x" << size << " - Average C++ time: " << average_time << " seconds\n";
    }

    resultsFile.close();
    std::cout << "Results saved to " << SAVE_PATH << "/results.csv" << std::endl;

    return 0;
}