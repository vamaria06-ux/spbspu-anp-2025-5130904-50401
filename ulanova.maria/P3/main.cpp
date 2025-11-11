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
}
