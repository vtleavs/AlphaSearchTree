#include "headers/AlphaSearch.h"
#include "headers/FileParse.h"

int main()
{
  std::cout << "Loading Dictionary...";

  AlphaSearchTree dictionary;
  dictionary.loadFromFile("../model/dictionary.ast");

  std::cout << "Enter part of a word:\n";
  std::string inString;
  do
  {
    std::cout << " > ";
    inString = "";
    std::cin >> inString;
    if(inString == "/q")
      break;

    std::vector<std::string> completions = dictionary.autoComplete(stripPunctuation(inString), 5);

    for(std::string s : completions)
      std::cout << s << " ";
    std::cout << "\n\n";



  } while (inString != "/q");

  std::cout << "quitting..\n";

  return 0;
}
