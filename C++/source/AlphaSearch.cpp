#ifndef _ALPHASEARCH_CPP_
#define _ALPHASEARCH_CPP_

#include "../headers/AlphaSearch.h"

ASNode* ASNode::getNextNode(char c){

  // Finding the next node in sequence for a character
  for(int i=0; i<children.size(); ++i){
    if(children[i].getValue() == c){
      return &children[i];
    }
  }

  // The character does not have a corresponding node if got here
  return NULL;

}

// The length argument refers to depth of the children of our ASNode
std::vector<std::string> ASNode::getAllChildrenOfLength(const int & length){
  // Means every next node is the final node and we can start returning strings
  if(length == 1){
    std::vector<std::string> terminators;
    for(int i=0; i < children.size(); ++i){
      // Adding character to terminators if valid terminating node
      if(children[i].isTerm()){
        std::string temp;
        temp.push_back(children[i].getValue());
        terminators.push_back(temp);
      }
    }

    return terminators;
  }

  else{
    // Recursively iterate over each node to extract children
    for(int i=0; i < children.size(); ++i){
      std::vector<std::string> validWords = children[i].getAllChildrenOfLength(length-1);

      // Inserting parent node value at the front to complete a word
      for(auto itr = validWords.begin(); itr != validWords.end(); ++itr)
        (*itr) = children[i].getValue() + (*itr);

      return validWords;
    }
  }
}

void ASNode::readFromFile(std::ifstream & ifs)
{
  while(ifs.peek() != EOF)
  {
    std::string tag = "";

    if(ifs.peek() == '<')
    {
      tag = getTag(ifs);

      if(tag == "/n")
          return;

      else if(tag == "n")
      {
        ASNode newNode;
        newNode.readFromFile(ifs);
        children.push_back(newNode);
      }

      else if(tag == "v")
        value = ifs.get();

      else if(tag == "nh")
      {
        std::string nhResult = "";
        while(ifs.peek() != '<')
        {
          nhResult += ifs.get();
        }
        numHits = atoi(nhResult.c_str());
      }

      else if(tag == "sh")
      {
        std::string shResult = "";
        while(ifs.peek() != '<')
        {
          shResult += ifs.get();
        }
        sumHits = atoi(shResult.c_str());
      }

      else if(tag == "/t")
        term = true;
    }

    else if(ifs.peek() == '\n' || ifs.peek() == ' ')
      char dump = ifs.get();



  }
}

std::string ASNode::getTag(std::ifstream & ifs)
{
  std::string result = "";

  char dump = ifs.get();
  while(ifs.peek() != EOF && ifs.peek() != '>')
  {
    result += ifs.get();
  }
  dump = ifs.get();
  return result;
}

void ASNode::writeToFile(std::ofstream & ofs)
{
  sortChildren();

  if(value != '\0')
  {
    ofs << "\n<n>";
    std::string valString(1, value);
    ofs << "<v>" + valString + "</v>";
    ofs << "<nh>" + std::to_string(numHits) + "</nh>";
    ofs << "<sh>" + std::to_string(sumHits) + "</sh>";
    if(term)
      ofs << "</t>";
  }

  for(auto itr = children.begin(); itr != children.end(); itr++)
  {
    (*itr).writeToFile(ofs);
  }

  if(value != '\0')
    ofs << "</n>";
}

void AlphaSearchTree::saveToFile(std::string filename)
{
  std::ofstream ofs(filename);
  ofs << "<head>\n";
  head.writeToFile(ofs);
  ofs << "\n</head>";
  ofs.close();
}

void AlphaSearchTree::loadFromFile(std::string filename)
{
  std::ifstream ifs;
  ifs.open (filename, std::ifstream::in);
  head.readFromFile(ifs);
}


/**
* Generates a list of possible completion stubs for a given partial string
*
* @param partial - An incomplete word to be evaluated
* @return A vector containing a number of possible completion stubs
*/
std::vector<std::string> ASNode::autoComplete(std::string partial)
{
    // Grabs the last node corresponding to the final letter
    //head.



  // Backward phase: guesses stubs either until it has found
  // enough (currently 5) or until it times out (currently 30 attempts)
  // std::vector<std::string> stubs;
  // if(partial.length() == 0)
  // {
  //   int attempts = 0;
  //   while(stubs.size() < 5 && attempts < 30)
  //   {
  //     std::string stub = guessStub();
  //     if(std::find(stubs.begin(), stubs.end(), stub) == stubs.end() && stub != "\0")
  //       stubs.push_back(stub);
  //     attempts++;
  //   }
  //

  // Foreward phase: recursively navigates to the node at the end of
  // the partial string
  // for(auto itr = children.begin(); itr != children.end(); itr++)
  // {
  //   if((*itr).getValue() == partial[0])
  //   {
  //     return (*itr).autoComplete(partial.substr(1));
  //   }
  // }

  // return stubs;
}

/**
* Finds a single stub using a random function on the weights of the child nodes
*
* @return A single stub as a string
*/
std::string ASNode::guessStub()
{
  if(children.empty())
    return "";

  ASNode node('\0');

  // because the nodes are sorted by weight, we can just check to see if
  // each weight is greater than a random number between 0, and the sum
  // of the hits of all the children.
  sortChildren();
  int random = (int)(((double)rand() / RAND_MAX) * sumHits);
  for(auto itr = children.begin(); itr != children.end(); itr++)
  {
    if(random <= (*itr).getNumHits())
    {
      ASNode node = *itr;
      std::string st(1, node.getValue());
      std::string result = "" + st + node.guessStub();
      return result;
    }
  }

  return "";
}

/**
* A comparison function used to sort nodes by weight
*
* @param i - The first Node
* @param j - The second Node
* @return True if node i has a lower weight than node j
*/
bool ASNode::compareNodes(ASNode i, ASNode j)
{
  return i.getNumHits() < j.getNumHits();
}

/**
* Sorts the child nodes into smallest to largest by weight
*/
void ASNode::sortChildren()
{
  std::sort(children.begin(), children.end(), compareNodes);
}

/**
* Recursively adds a string to this node
*
* Checks front beginning character in the word.  Creates a child if needed.
* Then adds the remainder of the word to the appropriate child.  If the end
* of the string is reached, the node terminator is set to true.
*
* @param word - The string to be added to this node and its children
*/
void ASNode::addWord(std::string word)
{
  if(word.length() == 0)
  {
    term = true;
    return;
  }

  for(int i = 0; i < children.size(); i++)
  {
    if(children[i].getValue() == word[0])
    {
      children[i].addWord(word.substr(1));
      children[i].hit();

      sumHits++;
      return;
    }
  }

  ASNode newNode(word[0]);
  newNode.addWord(word.substr(1));
  children.push_back(newNode);
  sumHits++;
}

/**
* Checks to see if a string is contained in the sub-tree
*
* Recursively checks that each character in the string is a valid node.
*
* @param word The string to be checked for validity
* @return true if word = Ɛ and this node is a terminator, false if there is
*   no appropriate child or word = Ɛ and this node is not a terminator
*/
bool ASNode::isWord(std::string word)
{
  if(word.length() == 0)
    return term == true;

  for(int i = 0; i < children.size(); i++)
  {
    if(children[i].getValue() == word[0])
    {
      return children[i].isWord(word.substr(1));
    }
  }

  return false;
}

int ASNode::wordWeight(std::string word)
{
  if(word.length() == 0)
    return numHits;

  for(int i = 0; i < children.size(); i++)
  {
    if(children[i].getValue() == word[0])
      return numHits + children[i].wordWeight(word.substr(1));
  }

  return 0;
}

/**
* Prints the value of this node, and all of its children.
*
* This method is recursive, and should only be used for debugging
*/
void ASNode::print()
{
  std::cout << value << ": ";
  for (auto i = children.begin(); i != children.end(); ++i)
  {
    std::cout << (*i).getValue() << ' ';
  }

  for (auto i = children.begin(); i != children.end(); ++i)
  {
    std::cout << std::endl;
    (*i).print();

  }
}

#endif
