#ifndef _FILEPARSE_CPP_
#define _FILEPARSE_CPP_

#include "../headers/FileParse.h"

/**
* Splits a text file into a vector of strings
*
* @param filename - The name of the text file to load and split
* @return A vector of words contained in file filename
*/
std::vector<std::string> splitFile(std::string filename)
{
  std::ifstream file(filename);
  std::vector<std::string> result;

  while(!file.eof())
  {
    std::string temp;
    file >> temp;
    temp = stripPunctuation(temp);
    if(temp.length() > 0)
      result.push_back(temp);
  }

  file.close();
  return result;
}

void appendFile(std::string filename, std::string word)
{
  std::vector<std::string> words = splitFile(filename);
  words.push_back(stripPunctuation(word));
  std::sort(words.begin(), words.end());
  std::ofstream output(filename);

  for(std::string s : words)
  {
    output << s << "\n";
  }

  output.close();

}

/**
* Removes all punctuation from a string except for "-" and "`"
*
* Also removes spaces and makes all words uppercase
*
* @param word - The string to be stripped
* @return The resulting string
*/
std::string stripPunctuation(std::string word)
{
  std::stringstream wordStream(word);

  std::string result = "";

  for(char c : word)
  {
    if(c != '.'
      && c != ','
      && c != '?'
      && c != '!'
      && c != '/'
      && c != '\\'
      && c != ' '
      && c != '('
      && c != ')'
      && c != '*'
      && c != '&'
      && c != '^'
      && c != '{'
      && c != '}'
      && c != '['
      && c != ']'
      && c != '|'
      && c != '"'
      && c != '<'
      && c != '>'
      && c != '%'
      && c != '@'
      && c != '#'
      && c != '+'
      && c != '_'
      && c != '='
      && c != '~'
      && c != ':'
      && c != ';'
      && c != '\t'
      && c != '\n'
      && c != '-'
      && c != '`'
    ){
      if(c >= 97)
        c = c - 32;
      result += c;
    }

  }

  return result;
}

#endif
