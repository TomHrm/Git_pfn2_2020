#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>
#include <set>
#include <fstream>

typedef std::set<std::string> str_set;

str_set *file2wordset(std::ifstream *infile);

#endif
