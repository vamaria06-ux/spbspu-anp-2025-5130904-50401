#include <iostream>
#include <cstring>
#include <cctype>
namespace ulanova
{
  void excludeChars(const char* src, const char* exclude, char* result, size_t resultSize)
  {
    if (!src || !exclude || !result || resultSize == 0)
    {
      result[0] = '\0';
      return;
    }
    size_t idx = 0;
    for (const char* p = src; *p != '\0'; ++p)
    {
      int found = 0;
      for (const char * q = exclude; *q != '\0'; ++q)
      {
        if(*p == *q)
        {
          found = 1;
          break;
        }
      }
      if (!found)
      {
        if (idx >= resultSize - 1)
        {
          result[0] = '\0';
          return;
        }
        result[idx++] = *p;
      }
    }
    result[idx] = '\0';
  }
  void appendDigits(const char* src,const char* digitsSrc, char* result, size_t resultSize)
  {
    if (!src || !digitsSrc || !result || resultSize == 0)
    {
      result[0] = '\0';
      return;
    }
    size_t idx = 0;
    for ( const char* p = src; *p != '\0'; ++p)
    {
      if (idx >= resultSize - 1)
      {
        result[0] = '\0';
        return;
      }
      result[idx++] = *p;
    }
    for (const char* q = digitsSrc; *q != '\0'; ++q)
    {
      if(std::isdigit(*q))
      {
        int exists = 0;
        for (size_t i = 0; i < idx; ++i)
        {
          if (result[i] == *q)
          {
            exists = 1;
            break;
          }
        }
        if (!exists)
        {
          if (idx >= resultSize - 1)
          {
            result[idx++] = *q;
          }
        }
      }
    }
    result[idx] = '\0';
  }
}
int main()
{
  const size_t max_input_size = 1024;
  const size_t max_result_size = 2048;
  char input[max_input_size];
  char result[max_result_size];
  if (!std::cin.getline(input, max_input_size))
  {
    std::cerr << "Error: input long\n";
    return 1;
  }
  const char* excludeStr = "abc";
  const char* digitsStr = "g1h2k";
  ulanova::excludeChars(input, excludeStr, result , max_result_size);
  if (result[0] == '\0')
  {
    std::cerr << "Error: result bufer over\n";
    return 1;
  }
  std::cout << result << '\n';
  return 0;
}
