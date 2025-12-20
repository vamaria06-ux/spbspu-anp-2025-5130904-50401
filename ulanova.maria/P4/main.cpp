#include <iostream>
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
}
