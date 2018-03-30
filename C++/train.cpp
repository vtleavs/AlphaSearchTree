#include "headers/AlphaSearch.h"
#include "headers/FileParse.h"

int main(int argc, char const *argv[])
{
  if(argc < 2)
  {
    std::cout << "No input files specified.\n";
    return -1;
  }

  AlphaSearchTree dictionary;
  std::cout << "Loading Dictionary from AST... ";
  dictionary.loadFromFile("../model/dictionary.ast");
  std::cout << "\n\n";

  for(int i = 1; i < argc; i++)
  {
    std::string s(argv[i]);
    std::string path = "../model/input/" + s;
    std::vector<std::string> words = splitFile(path);
    std::cout << "Adding words from " << s << " into tree... ";
    for(std::string word : words)
      dictionary.addWord(word);
    std::cout << "\n" << words.end() - words.begin() << " words loaded into tree.\n\n";
  }

  std::cout << "Saving Dictionary to AST... ";
  dictionary.saveToFile("../model/dictionary.ast");

  return 0;

}
