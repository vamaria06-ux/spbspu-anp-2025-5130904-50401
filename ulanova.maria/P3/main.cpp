#include <iostream>

int main(int argc, char ** argv) {
    namespace ul = ulanova;
    ul:: size_t rows=0, cols=0;
    //Проверка параметров входных данных
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    } else if (argc > 4){
        std::cerr << "Too many arguments\n";
        return 1;
    }
    try {
        if( std::stoi(argv[1])!='1' && std::stoi(argv[1])!='2'){
            std::cerr << "Wrong arguments \n";
            return 1;
        }
    } catch (const std::invalid_argument& e){
        std::cerr << "First parametr is not a number \n";
        return 1;
    }
    //Открываем файл
    std::ifstream input(argv[2]);
    if (!input) {
        std::cerr << "Cannot open input file\n";
        return 1;
    }
    //чтение матрицы
    if (!(input >> rows >> cols)) {
        std::cerr << "Invalid matrix dimensions\n";
        return 2;
    }
    
    if (rows < 0 || cols < 0) {
        std::cerr << "Negative matrix dimensions\n";
        return 2;
    }
    
    std::string num_str = argv[1];
    int num;
    num = std::stoi(num_str);
    
     // Проверяем ограничение для фиксированного массива
    if (num == 1 && rows * cols > 10000) {
        std::cerr << "Matrix too large for fixed array\n";
        return 2;
    }
    
    // Создаем матрицу
    int** matrix = nullptr;
    
    if (rows > 0 && cols > 0) {
        // Выделяем память для строк
        matrix = new int*[rows];
        // Выделяем память для каждого столбца
        for (int i = 0; i < rows; i++) {
            matrix[i] = new int[cols];
        }
        
        // Читаем элементы
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (!(input >> matrix[i][j])) {
                    std::cerr << "Invalid matrix element\n";
                    // Освобождаем память перед выходом
                    for (int k = 0; k <= i; k++) {
                        delete[] matrix[k];
                    }
                    delete[] matrix;
                    return 2;
                }
            }
        }
    }
    input.close();
    // Обрабатываем матрицу - ищем седловые элементы
    int saddle_count = 0;
    
    if (rows > 0 && cols > 0) {
        for (int i = 0; i < rows; i++) {
            // Находим минимум в строке
            int min_val = matrix[i][0];
            int min_col = 0;
            for (int j = 1; j < cols; j++) {
                if (matrix[i][j] < min_val) {
                    min_val = matrix[i][j];
                    min_col = j;
                }
            }
            // Проверяем, является ли этот элемент максимумом в столбце
            bool is_max = true;
            for (int k = 0; k < rows; k++) {
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
    
    // Записываем результат
    std::ofstream output(output_file);
    if (!output.is_open()) {
        std::cerr << "Cannot open output file\n";
        // Освобождаем память перед выходом
        if (matrix != nullptr) {
            for (int i = 0; i < rows; i++) {
                delete[] matrix[i];
            }
            delete[] matrix;
        }
        return 2;
    }
    
    output << saddle_count;
    output.close();
    
    // Освобождаем память
    if (matrix != nullptr) {
        for (int i = 0; i < rows; i++) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
    
    return 0;
}
 //
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace ul = ulanova;

bool checkArguments(int argc, char* argv[], int& taskNum, std::string& inputFile, std::string& outputFile) {
    if (argc != 4) {
        std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << std::endl;
        return false;
    }
    
    try {
        taskNum = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "First parameter is not a number" << std::endl;
        return false;
    }
    
    if (taskNum != 1 && taskNum != 2) {
        std::cerr << "First parameter is out of range" << std::endl;
        return false;
    }
    
    inputFile = argv[2];
    outputFile = argv[3];
    return true;
}

bool readMatrixFromFile(const std::string& filename, int**& matrix, int& rows, int& cols) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open input file" << std::endl;
        return false;
    }
    
    if (!(file >> rows >> cols)) {
        std::cerr << "Invalid matrix dimensions" << std::endl;
        return false;
    }
    
    if (rows < 0 || cols < 0) {
        std::cerr << "Negative matrix dimensions" << std::endl;
        return false;
    }
    
    if (rows == 0 || cols == 0) {
        matrix = nullptr;
        return true;
    }
    
    matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!(file >> matrix[i][j])) {
                std::cerr << "Invalid matrix element at (" << i << "," << j << ")" << std::endl;
                return false;
            }
        }
    }
    
    file.close();
    return true;
}

int countSaddlePoints(int** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) return 0;
    
    int count = 0;
    
    for (int i = 0; i < rows; i++) {
        int minInRow = matrix[i][0];
        int minColIndex = 0;
        
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] < minInRow) {
                minInRow = matrix[i][j];
                minColIndex = j;
            }
        }
        
        bool isMaxInCol = true;
        for (int k = 0; k < rows; k++) {
            if (matrix[k][minColIndex] > minInRow) {
                isMaxInCol = false;
                break;
            }
        }
        
        if (isMaxInCol) {
            count++;
        }
    }
    
    return count;
}

void transformMatrixSpiral(int** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) return;
    
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;
    int decrement = 1;
    
    while (top <= bottom && left <= right) {
        for (int i = left; i <= right; i++) {
            matrix[top][i] -= decrement;
            decrement++;
        }
        top++;
        
        for (int i = top; i <= bottom; i++) {
            matrix[i][right] -= decrement;
            decrement++;
        }
        right--;
        
        if (top <= bottom) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] -= decrement;
                decrement++;
            }
            bottom--;
        }
        
        if (left <= right) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] -= decrement;
                decrement++;
            }
            left++;
        }
    }
}

bool writeResultToFile(const std::string& filename, int result) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open output file" << std::endl;
        return false;
    }
    
    file << result;
    file.close();
    return true;
}

bool writeMatrixToFile(const std::string& filename, int** matrix, int rows, int cols) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open output file" << std::endl;
        return false;
    }
    
    file << rows << " " << cols;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            file << " " << matrix[i][j];
        }
    }
    
    file.close();
    return true;
}

void freeMatrix(int** matrix, int rows) {
    if (matrix == nullptr) return;
    
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

bool checkFixedSizeLimit(int taskNum, int rows, int cols) {
    if (taskNum == 1 && rows * cols > 10000) {
        std::cerr << "Matrix too large for fixed array" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int taskNum;
    std::string inputFile, outputFile;
    
    if (!ul::checkArguments(argc, argv, taskNum, inputFile, outputFile)) {
        return 1;
    }
    
    int** matrix = nullptr;
    int rows, cols;
    
    if (!ul::readMatrixFromFile(inputFile, matrix, rows, cols)) {
        return 2;
    }
    
    if (!ul::checkFixedSizeLimit(taskNum, rows, cols)) {
        ul::freeMatrix(matrix, rows);
        return 2;
    }
    
    // Выбор операции в зависимости от имени программы
    std::string programName = argv[0];
    
    if (programName.find("saddle") != std::string::npos) {
        // Задача 1: подсчет седловых элементов
        int result = ul::countSaddlePoints(matrix, rows, cols);
        if (!ul::writeResultToFile(outputFile, result)) {
            ul::freeMatrix(matrix, rows);
            return 2;
        }
    } else if (programName.find("spiral") != std::string::npos) {
        // Задача 2: спиральное преобразование
        ul::transformMatrixSpiral(matrix, rows, cols);
        if (!ul::writeMatrixToFile(outputFile, matrix, rows, cols)) {
            ul::freeMatrix(matrix, rows);
            return 2;
        }
    } else {
        // По умолчанию: спиральное преобразование
        ul::transformMatrixSpiral(matrix, rows, cols);
        if (!ul::writeMatrixToFile(outputFile, matrix, rows, cols)) {
            ul::freeMatrix(matrix, rows);
            return 2;
        }
    }
    
    ul::freeMatrix(matrix, rows);
    return 0;
}





























}
