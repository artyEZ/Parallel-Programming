import numpy as np
import matplotlib.pyplot as plt
import time
import os


def read_matrix_from_file(filepath):
    with open(filepath, 'r') as file:
        _size = int(file.readline())
        matrix = np.loadtxt(file, dtype=int, max_rows=_size)
    return matrix


def extract_execution_time(filepath):
    with open(filepath, 'r') as file:
        lines = file.readlines()
        time_line = lines[-2].strip()
        execution_time = float(time_line.split(": ")[1].split(" ")[0])
    return execution_time


if __name__ == "__main__":

    save_path = "C:/Users/artyo/Desktop/Учеба/Параллельное программирование/Lab1"

    sizes = [10, 100, 1000]

    for size in sizes:
        matrixA_path = os.path.join(save_path, f"matrixA_size_{size}.txt")
        matrixB_path = os.path.join(save_path, f"matrixB_size_{size}.txt")

        matrixA = read_matrix_from_file(matrixA_path)
        matrixB = read_matrix_from_file(matrixB_path)

        result_path = os.path.join(save_path, f"result_size_{size}.txt")
        cpp_time = extract_execution_time(result_path)

        start = time.time()
        python_result = np.dot(matrixA, matrixB)
        end = time.time()
        python_time = end - start

        print(f"Matrix size: {size}x{size}")
        print(f"C++ execution time: {cpp_time} seconds")
        print(f"Python execution time: {python_time} seconds")
        print("-" * 30)

        plt.figure(figsize=(8, 6))
        labels = ['C++', 'Python']
        times = [cpp_time, python_time]

        plt.bar(labels, times, color=['blue', 'red'])
        plt.xlabel('Implementation')
        plt.ylabel('Execution Time (seconds)')
        plt.title(f'Execution Time Comparison for {size}x{size} Matrices')
        plt.grid(True)

        histogram_path = os.path.join(save_path, f"histogram_size_{size}.png")
        plt.savefig(histogram_path)
        plt.close()

        print(f"Histogram saved to {histogram_path}")