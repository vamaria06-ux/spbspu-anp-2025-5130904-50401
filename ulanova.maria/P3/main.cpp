#include <iostream>
#include <fstream>
#include <cstddef>
#include <stdexcept>
namespace ulanova {
  using s_t = size_t;
  int ** read_creat_Matrix(std::istream& input, char const ** argv, s_t & rows, s_t & cols) {
    if (!(input >> rows >> cols)) {
      std::cerr << "Invalid matrix\n";
      return nullptr;
    }
    if (rows == 0 || cols == 0){
      return nullptr;
    }
    size_t num = 0;
    try {
      num = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e ) {
      std::cerr << "Invalid argument\n";
      return nullptr;
    }
    if (num == 1 && rows * cols > 10000) {
      std::cerr << "Matrix too large for fixed array\n";
      return nullptr;
    }
    int ** matrix = nullptr;
    if (rows > 0 && cols > 0) {
      matrix = new int*[rows];
      for (s_t i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
      }
      for (s_t i = 0; i < rows; i++) {
        for (s_t j = 0; j < cols; j++) {
          if (!(input >> matrix[i][j])) {
            std::cerr << "Invalid matrix element\n";
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
  std::ostream & outputMatrix(std::ostream& out, int** matrix, s_t rows, s_t cols) {
    for (s_t i = 0; i < rows; ++i) {
      for (s_t j = 0; j < cols; ++j) {
        out << matrix[i][j] << " ";
      }
    }
    return out;
  }
  int sedlMatrix(int** matrix, s_t rows, s_t cols) {
    int saddle_count = 0;
    if (rows > 0 && cols > 0) {
      for (s_t i = 0; i < rows; i++) {
        int min_val = matrix[i][0];
        s_t min_col = 0;
        for (s_t j = 1; j < cols; j++) {
          if (matrix[i][j] < min_val) {
            min_val = matrix[i][j];
            min_col = j;
          }
        }
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
  void spiralTransform(int ** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0){
      return;
    }
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;
    int counter = 1;
    while (top <= bottom && left <= right) {
      for (int j = left; j <= right; j++) {
        matrix[top][j] -= counter;
        counter++;
      }
      top++;
      for (int i = top; i <= bottom; i++) {
        matrix[i][right] -= counter;
        counter++;
      }
      right--;
      if (top <= bottom) {
        for (int j = right; j >= left; j--) {
          matrix[bottom][j] -= counter;
          counter++;
        }
        bottom--;
      }
      if (left <= right) {
        for (int i = bottom; i >= top; i--) {
          matrix[i][left] -= counter;
          counter++;
        }
        left++;
      }
    }
  }
  int ** spiralTransformCopy(int ** matrix, int rows, int cols) {
    if (rows == 0 || cols == 0) {
      return nullptr;
    }
    int ** result = new int*[rows];
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

int main(int argc, char const** argv) {
  using s_t = size_t;
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
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  namespace ula = ulanova;
  s_t rows = 0;
  s_t cols = 0;
  int ** matrix = ula::read_creat_Matrix(input, argv, rows, cols);
  input.close();
  if (!matrix && (rows > 0 || cols > 0)) {
    return 2;
  }
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    for ( s_t i = 0; i < rows; i++) {
      delete[] matrix[i];
    }
    delete[] matrix;
    return 1;
  }
  output << ula::sedlMatrix(matrix, rows, cols) << '\n';
  int ** transformed = ula::spiralTransformCopy(matrix, rows, cols);
  if (transformed){
    ula::outputMatrix(output,transformed,rows,cols);
    for (ulanova::s_t i = 0; i < rows; i++) {
      delete[] transformed[i];
    }
    delete[] transformed;
  }

  if (matrix) {
    for (s_t i = 0; i < rows; i++) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }
  return 0;
}
