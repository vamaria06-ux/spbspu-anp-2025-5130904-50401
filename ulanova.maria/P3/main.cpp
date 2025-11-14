#include <iostream>
#include <fstream>

namespace ulanova
{
    using s_t =size_t;
    int* readMatrix(std::istream& input,char const** argv,s_t& rows, s_t& cols);
    std::ostream outputMatrix(std::ostream& out, const int* matrix, s_t rows, s_t cols);
    int sedlMatrix (const int* matrix, s_t rows, s_t cols);
    void spiralTransform(int** matrix, int rows, int cols);
    int** spiralTransformCopy(int** matrix, int rows, int cols);
}

int main(int argc,char const** argv)
{ //проыверка аргументов
    namespace ulanova;
    ulanova::s_t rows=0, cols=0;
    if (argc < 4) {
        std::cerr << "Not enough arguments\n";
        return 1;
    } else if (argc > 4){
        std::cerr << "Too many arguments\n";
        return 1;
    } try {
        if( std::stoi(argv[1])!='1' && std::stoi(argv[1])!='2'){
            std::cerr << "Wrong arguments \n";
            return 1;
        }
    } catch (const std::invalid_argument& e){
        std::cerr << "First parametr is not a number \n";
        return 1;
    }
    // input matrix
    std::ifstream input(argv[2]);
    if (!input) {
        std::cerr << "Cannot open input file\n";
        return 1;
    }
    int* matrix = ulanova::readMatrix(input,char const** argv, rows, cols);
    input.close();
    int (!matrix){
        return 2;
    }
    std::ofstream output(argv[3]);
    output << ulanova::sedlMatrix(matrix, rows, cols) << '\n';
    output << ulanova::spiralTransformCopy( matrix, rows, cols) << '\n';
    delete[] matrix;   
}    
    

int* ulanova::readMatrix(std::istream& input, char const** argv, s_t& rows, s_t& cols){
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
    }
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
    return matrix;
} 
std::ostream ulanova::outputMatrix(std::ostream& out, const int* matrix, s_t rows, s_t cols){
    for (s_t i = 0; i < rows; ++i){
        for (s_t j = 0; j < cols; ++j){
            output << matrix[i * cols + j] << " ";
        }
        output << "\n";
    }
    return output:
}

int  ulanova::sedlMatrix (const int* matrix, s_t rows, s_t cols){
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
    return saddle_count;
}
void ulanova::spiralTransform(int** matrix, int rows, int cols) {
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
int** ulanova::spiralTransformCopy(int** matrix, int rows, int cols) {
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


















}
