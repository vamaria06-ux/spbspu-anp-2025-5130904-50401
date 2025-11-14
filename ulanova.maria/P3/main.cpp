#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

namespace ulanova
{
    using s_t = size_t;
    int** readMatrix(std::istream& input, char const** argv, s_t& rows, s_t& cols);
    std::ostream& outputMatrix(std::ostream& out, int** matrix, s_t rows, s_t cols);
    int sedlMatrix(int** matrix, s_t rows, s_t cols);
    void spiralTransform(int** matrix, int rows, int cols);
    int** spiralTransformCopy(int** matrix, int rows, int cols);
}

int main(int argc, char const** argv)
{
    // проверка аргументов
    ulanova::s_t rows = 0, cols = 0;
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    } else if (argc > 4) {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    
    try {
        int num = std::stoi(argv[1]);
        if (num != 1 && num != 2) {
            std::cerr << "Wrong arguments \n";
            return 1;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "First parameter is not a number \n";
        return 1;
    }
    
    // input matrix
    std::ifstream input(argv[2]);
    if (!input) {
        std::cerr << "Cannot open input file\n";
        return 1;
    }
    
    int** matrix = ulanova::readMatrix(input, argv, rows, cols);
    input.close();
    
    if (!matrix) {
        return 2;
    }
    
    std::ofstream output(argv[3]);
    if (!output) {
        std::cerr << "Cannot open output file\n";
        // Освобождаем память
        for (ulanova::s_t i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
        return 1;
    }
    
    output << ulanova::sedlMatrix(matrix, rows, cols) << '\n';
    
    int** transformed = ulanova::spiralTransformCopy(matrix, rows, cols);
    ulanova::outputMatrix(output, transformed, rows, cols);
    
    // Освобождаем память
    for (ulanova::s_t i = 0; i < rows; i++) {
        delete[] matrix[i];
        delete[] transformed[i];
    }
    delete[] matrix;
    delete[] transformed;
    
    return 0;
}

int** ulanova::readMatrix(std::istream& input, char const** argv, s_t& rows, s_t& cols)
{
    // чтение матрицы
    if (!(input >> rows >> cols)) {
        std::cerr << "Invalid matrix dimensions\n";
        return nullptr;
    }
    
    if (rows <= 0 || cols <= 0) {
        std::cerr << "Invalid matrix dimensions\n";
        return nullptr;
    }
    
    std::string num_str = argv[1];
    int num;
    try {
        num = std::stoi(num_str);
    } catch (const std::exception& e) {
        std::cerr << "Invalid argument: " << e.what() << "\n";
        return nullptr;
    }
    
    // Проверяем ограничение для фиксированного массива
    if (num == 1 && rows * cols > 10000) {
        std::cerr << "Matrix too large for fixed array\n";
        return nullptr;
    }
    
    // Создаем матрицу
    int** matrix = nullptr;
    if (rows > 0 && cols > 0) {
        // Выделяем память для строк
        matrix = new int*[rows];
        // Выделяем память для каждого столбца
        for (s_t i = 0; i < rows; i++) {
            matrix[i] = new int[cols];
        }
        
        // Читаем данные
        for (s_t i = 0; i < rows; i++) {
            for (s_t j = 0; j < cols; j++) {
                if (!(input >> matrix[i][j])) {
                    std::cerr << "Invalid matrix element at [" << i << "][" << j << "]\n";
                    // Освобождаем память перед выходом
                    for (s_t k = 0; k <= i; k++) {
                        delete[] matrix[k];
                    }
                    delete[] matrix;
                    return nullptr;
                }
            }
        }
    }
    return matrix;
}

std::ostream& ulanova::outputMatrix(std::ostream& out, int** matrix, s_t rows, s_t cols)
{
    for (s_t i = 0; i < rows; ++i) {
        for (s_t j = 0; j < cols; ++j) {
            out << matrix[i][j] << " ";
        }
        out << "\n";
    }
    return out;
}

int ulanova::sedlMatrix(int** matrix, s_t rows, s_t cols)
{
    int saddle_count = 0;
    if (rows > 0 && cols > 0) {
        for (s_t i = 0; i < rows; i++) {
            // Находим минимум в строке
            int min_val = matrix[i][0];
            s_t min_col = 0;
            for (s_t j = 1; j < cols; j++) {
                if (matrix[i][j] < min_val) {
                    min_val = matrix[i][j];
                    min_col = j;
                }
            }
            // Проверяем, является ли этот элемент максимумом в столбце
            bool is_max = true;
            for (s_t k = 0; k < rows; k++) {
                if (matrix[k][min_col] > min_val) {
                    is_max = false;
                    break;
                }
            }
            if (is_max) {
                saddle_count++;
            }
        }
    }
    return saddle_count;
}

void ulanova::spiralTransform(int** matrix, int rows, int cols)
{
    if (rows == 0 || cols == 0) return;
    
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;
    int counter = 1;
    
    while (top <= bottom && left <= right) {
        // Верхняя строка: слева направо
        for (int j = left; j <= right; j++) {
            matrix[top][j] -= counter;
            counter++;
        }
        top++;
        
        // Правый столбец: сверху вниз
        for (int i = top; i <= bottom; i++) {
            matrix[i][right] -= counter;
            counter++;
        }
        right--;
        
        // Нижняя строка: справа налево (если есть)
        if (top <= bottom) {
            for (int j = right; j >= left; j--) {
                matrix[bottom][j] -= counter;
                counter++;
            }
            bottom--;
        }
        
        // Левый столбец: снизу вверх (если есть)
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] -= counter;
                counter++;
            }
            left++;
        }
    }
}

int** ulanova::spiralTransformCopy(int** matrix, int rows, int cols)
{
    if (rows == 0 || cols == 0) return nullptr;
    
    // Создаем копию матрицы
    int** result = new int*[rows];
    for (int i = 0; i < rows; i++) {
        result[i] = new int[cols];
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix[i][j];
        }
    }
    
    spiralTransform(result, rows, cols);
    return result;
}
