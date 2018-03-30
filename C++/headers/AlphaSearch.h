
#ifndef _ALPHASEARCH_H_
#define _ALPHASEARCH_H_

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include "FileParse.h"

/**
* Represents a Node in the AlphaSearch Tree
*
* value - A char that represents the value of this node in the string
* term - A boolean terminator.  If a node is a terminator, then the string
*   of previous nodes makes up a valid word
* numHits - The number of times this node is used by words.  Used in the
*   calculation of the weight
* sumHits - The sum of the hits of the children
* children - A vector containing all of the children of this node
*
*/
class ASNode
{
  public:
    ASNode(): term(false), value('\0'), numHits(0), sumHits(0) {}
    ASNode(char val): term(false), value(val), numHits(0), sumHits(0) {}

    void addWord(std::string word);
    bool isWord(std::string word);

    void sortChildren();
    void hit() { numHits++; }

    int wordWeight(std::string word);

    void print();

    char getValue() { return value; }
    int getNumHits() { return numHits; }
    void setTerminator() { term = true; }
    bool isTerm() { return term; }

    ASNode* getNextNode(char c);

    // NOTE: Length means tree depth not word length
    std::vector<std::string> getAllChildrenOfLength(const int & length);

    std::vector<std::string> autoComplete(std::string partial);


    void writeToFile(std::ofstream & ofs);
    void readFromFile(std::ifstream & ifs);

    std::string getTag(std::ifstream & ifs);

  private:
    bool term;
    char value;
    int numHits;
    int sumHits;
    std::vector<ASNode> children;

    std::string guessStub();
    static bool compareNodes(ASNode i, ASNode j);
};

/**
* A character based constant-time dictionary search stucture
*
* Mostly acts a wrapper for the nodes.  Little to no actual functionality.
*
* head - An ASNode with a value of Ɛ
* size - The number of words added to this tree (currently doesn't take
*   repeat words into account)
*
*/
class AlphaSearchTree
{
  public:
    AlphaSearchTree(): head('\0'), size(0) { std::srand (time(NULL)); }
    void addWord(std::string word);
    bool isWord(std::string word);

    int wordWeight(std::string word);

    std::string trackedWord;

    std::vector<std::string> autoComplete(std::string partial, const int & correctionLimit);

    void print();

    int getSize() { return size; }

    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);

  private:
    ASNode head;
    int size;

};

inline int AlphaSearchTree::wordWeight(std::string word)
{
  return head.wordWeight(word);
}

/**
* Finds a list of possible complete words formed out of a partial word
*
* Checks to see whether the composition of the partial and each stub is valid
*
* @param partial - The partial word to be evaluated
* @return A vector of complete words formed from the partial
*/
inline std::vector<std::string> AlphaSearchTree::autoComplete(std::string partial, const int & correctionLimit)
{
  std::vector<std::string> results;

  // Empty string edge case
  if(partial.size() == 0)
    return results;

  // If partial is a word, it is automatically the first suggestion
  if(isWord(partial))
    results.push_back(partial);

  // Grabbing the last node in sequence, aborting if the partial word is not valid
  ASNode* currentNode = &head;
  for(int i=0; i< partial.size(); ++i){
    currentNode = (*currentNode).getNextNode(partial[i]);

    if(currentNode == NULL)
      return results;
  }

  // Now have a pointer to the last node in the word sequence
  for(int i=1; i <= correctionLimit && results.size() < 5; ++i){
    std::vector<std::string> completions = (*currentNode).getAllChildrenOfLength(i);

    for(int i=0; i < completions.size() && results.size() < 5; ++i)
      results.push_back(partial + completions[i]);
  }

  return results;

  // std::vector<std::string> stubs = head.autoComplete(partial);

  // for(std::string s : stubs)
  // {
  //   if(isWord(partial + s))
  //     results.push_back(partial + s);
  // }

  // return results;

}



/**
* Adds a new word to the tree
*
* Adds a string to the head node and increases the tree size by one
*
* word- the string to be added to the tree
*/
inline void AlphaSearchTree::addWord(std::string word)
{
  head.addWord(word);
  size++;
}

/**
* Checks whether a string is in the tree
*
* Checks to see if the head node accepts the string
*
* @param word - the string to check
* @return true if the tree contains the word, false otherwise
*/
inline bool AlphaSearchTree::isWord(std::string word)
{
  return head.isWord(word);
}

/**
* Prints the value of every node in the tree
*
* This method will print out the entire tree, which can be very large.  This
*   method should therefore only be used in debugging.
*/
inline void AlphaSearchTree::print()
{
  head.print();
}

#endif
