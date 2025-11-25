#include <iostream>
#include <fstream>

namespace ulanova
{
  const size_t max_s = 10000;
  int ** createMatrix(size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return nullptr;
    }
    int ** matrix = new int * [rows];
    for (size_t i = 0; i < rows; ++i)
    {
      matrix[i] = new int[cols];
    }
    return matrix;
  }

  void freeMatrix(int **& matrix, size_t rows)
  {
    if (matrix)
    {
      for (size_t i = 0; i < rows; ++i)
      {
        delete[] matrix[i];
      }
      delete[] matrix;
      matrix = nullptr;
    }
  }

  bool readMatrix(const char * filename, int **& matrix, size_t& rows, size_t& cols, bool isfixedsize)
  {
    std::ifstream input(filename);
    if (!input.is_open())
    {
      std::cerr << "Error: Cannot open file '" << filename << "'\n";
      return false;
    }
    input.peek();
    if (input.eof())
    {
      std::cerr << "Error: File '" << filename << "' is empty\n";
      return false;
    }
    if (!(input >> rows >> cols))
    {
      std::cerr << "Error: Invalid matrix dimensions in '" << filename << "'\n";
      return false;
    }
    if (rows == 0 || cols == 0)
    {
      matrix = nullptr;
      return true;
    }
    if (isfixedsize && (cols > 0 && rows > max_s / cols ))
    {
      std::cerr << "Error: Matrix exceeds maximum size (" << max_s << ")\n";
      return false;
    }
    matrix = createMatrix(rows, cols);
    if (!matrix)
    {
      return false;
    }
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < cols; j++)
      {
        if (!(input >> matrix[i][j]))
        {
          freeMatrix(matrix,rows);
          matrix = nullptr;
          return false;
        }
      }
    }
    return true;
  }

  size_t sedlMatrix(int ** matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || !matrix)
    {
      return 0;
    }
    size_t saddle_count = 0;
    if (rows > 0 && cols > 0)
    {
      for (size_t i = 0; i < rows; i++)
      {
        int min_val = matrix[i][0];
        int min_col = 0;
        for (size_t j = 1; j < cols; j++)
        {
          if (matrix[i][j] < min_val)
          {
            min_val = matrix[i][j];
            min_col = j;
          }
        }
        bool is_max = true;
        for (size_t k = 0; k < rows; k++)
        {
          if (matrix[k][min_col] > min_val)
          {
            is_max = false;
            break;
          }
        }
        if (is_max)
        {
          saddle_count++;
        }
      }
    }
    return saddle_count;
  }
 void spiralTransform(int ** matrix, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 || !matrix)
    {
      return;
    }
    int top = 0, bottom = rows - 1;
    int left = 0, right = cols - 1;
    int counter = 1;
    while (top <= bottom && left <= right)
    {
      for (int j = left; j <= right; j++)
      {
        matrix[top][j] -= counter;
        counter++;
      }
      top++;
      for (int i = top; i <= bottom; i++)
      {
        matrix[i][right] -= counter;
        counter++;
      }
      right--;
      if (top <= bottom)
      {
        for (int j = right; j >= left; j--)
        {
          matrix[bottom][j] -= counter;
          counter++;
        }
        bottom--;
      }
      if (left <= right)
      {
        for (int i = bottom; i >= top; i--)
        {

          matrix[i][left] -= counter;
          counter++;
        }
        left++;
      }
    }
  }
  int ** spiralTransformCopy(int ** matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || !matrix)
    {
      return nullptr;
    }
    int ** result = new int*[rows];
    if (!result)
    {
      return nullptr;
    }
    for (size_t i = 0; i < rows; i++)
    {
      result[i] = new int[cols];
      for (size_t j = 0; j < cols; j++)
      {
        result[i][j] = matrix[i][j];
      }
    }
    spiralTransform(result, rows, cols);
    return result;
  }
}
int main(int argc, char * argv[])
{
  if (argc != 4)
  {
    std::cerr << "Error arguments, need 3\n";
    return 1;
  }
  const char * mode = argv[1];
  if (mode[0] != '1' && mode[0] != '2')
  {
    std::cerr << "Error mode 1 or 2\n";
    return 1;
  }
  int ** matrix = nullptr;
  size_t rows, cols;
  bool isfixedsize = (mode[0] == '1');
  if (!ulanova::readMatrix(argv[2], matrix, rows, cols, isfixedsize))
  {
    return 2;
  }
  size_t resultsedlMatrix = ulanova::sedlMatrix(matrix, rows, cols);
  int** resultspiralTransform = ulanova::spiralTransformCopy(matrix, rows, cols);
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Error: Cannot open output file '" << argv[3] << "'\n";
    ulanova::freeMatrix(matrix, rows);
    ulanova::freeMatrix(resultspiralTransform, rows);
    return 3;
  }
  output << resultsedlMatrix<< "\n";
  for (size_t i = 0; i< rows; i++)
  {
    for (size_t j = 0; j < cols; j++ )
    {
      output << resultspiralTransform[i][j];
    }
    output << "\n";
  }
  ulanova::freeMatrix(matrix, rows);
  ulanova::freeMatrix(resultspiralTransform,rows);
  return 0;
}
