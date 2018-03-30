#ifndef _FILEPARSE_H_
#define _FILEPARSE_H_

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<std::string> splitFile(std::string filename);

void appendFile(std::string filename, std::string word);

std::string stripPunctuation(std::string word);

#endif
