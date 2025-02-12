#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <filesystem> 
#include <sstream>   

const std::string SAVE_PATH = "C:/Users/artyo/Desktop/Учеба/Параллельное программирование/Lab1"; 

std::vector<std::vector<int>> createSquareMatrix(int size) {
    std::random_device rd;  // Источник случайных чисел
    std::mt19937 gen(rd()); // Генератор Mersenne Twister
    std::uniform_int_distribution<> distrib(0, 99); // Равномерное распределение от 0 до 99

    std::vector<std::vector<int>> matrix(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = distrib(gen); 
        }
    }
    return matrix;
}

void writeMatrixToFile(const std::vector<std::vector<int>>& matrix, const std::string& filepath, double executionTime, int taskVolume) {
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

    file << "Execution time: " << executionTime << " seconds" << std::endl;
    file << "Task volume: " << taskVolume << " operations" << std::endl;

    file.close();
}

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
    int size;

    std::cout << "Enter the size of the square matrices: ";
    std::cin >> size;

    if (!std::filesystem::exists(SAVE_PATH)) {
        std::cerr << "Error: The specified directory does not exist." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> matrixA = createSquareMatrix(size);
    std::vector<std::vector<int>> matrixB = createSquareMatrix(size);

    // Попытка сделать f-string
    std::ostringstream oss;
    oss << SAVE_PATH << "/matrixA_size_" << size << ".txt";
    std::string matrixA_path = oss.str();

    oss.str(""); // Очищаем поток
    oss << SAVE_PATH << "/matrixB_size_" << size << ".txt";
    std::string matrixB_path = oss.str();

    oss.str(""); 
    oss << SAVE_PATH << "/result_size_" << size << ".txt";
    std::string result_path = oss.str();

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> result = multiplyMatrices(matrixA, matrixB);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    int taskVolume = size * size * size;

    writeMatrixToFile(matrixA, matrixA_path, elapsed.count(), taskVolume);
    writeMatrixToFile(matrixB, matrixB_path, elapsed.count(), taskVolume);
    writeMatrixToFile(result, result_path, elapsed.count(), taskVolume);

    std::cout << "Matrix multiplication completed successfully." << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Task volume: " << taskVolume << " operations" << std::endl;

    return 0;
}