#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>
#include "hashfunctions.hpp"
#include "tokenizer.hpp"

int main(int argc, const char* argv[])
{
  if (argc != 2)
  {
    std ::cerr << "Usage: " << argv[0] << " <filename>" << std ::endl;
    return EXIT_FAILURE;
  }
  std ::ifstream infile(argv[1]);
  if (infile.fail())
  {
    std ::cerr << argv[0] << ": cannot open file " << argv[1] << std ::endl;
    return EXIT_FAILURE;
  }

  str_set* wordset = file2wordset(&infile);
  unsigned int i;

  std::vector<double> collisions;

  for (i = 0; i < hashfunction_number(); i++)
  {
    const Hashfunction* hashfunction = hashfunction_get(i);
    std::map<unsigned int, unsigned int> occurences;

    for (auto word : *wordset)
    {
      unsigned int hash = hashfunction->func(word.c_str());
      if (occurences.count(hash) == 0)
      {
        occurences[hash] = 1;
      } else
      {
        occurences[hash]++;
      }
    }
    unsigned int sum = 0;

    for (auto hash : occurences)
    {
      sum += hash.second * hash.second;
    }

    collisions.push_back((double) sum / (double) occurences.size());
  }

  while (true)
  {
    unsigned int maxindex = 0;
    size_t i;
    for (i = 0; i < collisions.size(); i++)
    {
      if (collisions[i] != 0)
      {
        if (collisions[maxindex] == 0)
        {
          maxindex = i;
        } else if (collisions[i] - 1e-8 < collisions[maxindex] &&
                   collisions[maxindex] < collisions[i] + 1e-8)
        {
          if (strcmp(hashfunction_get(i)->name,
                     hashfunction_get(maxindex)->name) < 0)
          {
            maxindex = i;
          }
        } else if (collisions[i] < collisions[maxindex])
        {
          maxindex = i;
        }
      }
    }
    if (collisions[maxindex] == 0)
    {
      break;
    }

    std::cout << hashfunction_get(maxindex)->name << "\t"
              << std::setprecision(9) << collisions[maxindex] << std::endl;
    collisions[maxindex] = 0;
  }

  return EXIT_SUCCESS;
}
