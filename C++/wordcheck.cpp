#include "headers/AlphaSearch.h"
#include "headers/FileParse.h"

int main() {

  std::cout << "Loading Dictionary...";

  AlphaSearchTree dictionary;
  dictionary.loadFromFile("../model/dictionary.ast");

  std::cout << "Enter words to check whether they are in the dictionary:\n";
  std::string inString;
  do
  {
    std::cout << " > ";
    inString = "";
    std::cin >> inString;
    if(inString == "/q")
      break;

    if(dictionary.isWord(stripPunctuation(inString)))
      std::cout << "'" << inString << "' is a word.\n";
    else
      std::cout << "'" << inString << "' is not a word.\n";
  } while (inString != "/q");

  std::cout << "quitting..\n";

  return 0;
}
