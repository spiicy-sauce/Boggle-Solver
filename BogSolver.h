
#ifndef BOGSOLVER
#define BOGSOLVER

#include <iostream>
#include <cstring>
#include "Dictionary.h"
#include "BogWordList.h"
#include "Matrix.h"
using namespace std;

class BogSolver
{

public:
    
    BogSolver();

    ~BogSolver();

    bool readDict();

    bool readBoard();

    bool solve(); // search board for words in dict

    int numWords(); // returns number of words found

    int numWords(int len); // number of words of length len

    BogWordList* getWords(); // returns all words found

    BogWordList* getWords(int len); // returns words of len len

    void printWords(); // print all words in HBF

    void printWords(int len); // print len-length words in HBF

    void listWords(); // print just the text, no coords

    void listWords(int len); // just the text, no coords

private:

    /* MEMBER DATA VARIABLES */

    Dictionary dict; // input words

    Dictionary foundDict; // found words

    BoardNode* board; // board letters

    bool** bogMatrix; // links from each piece to those around it

    int height, width, maxWordSize, foundCap; // size information

    BogWord currWord; // current string segment used to search

    BogWordList found; // stores found BogWords

    Matrix* MatrixGenerator; // creates matrix

    /* HELPER FUNCTIONS */

    void makeMatrix();

    void wordSearch(int); // searches for words beginning at root

    bool initFound(); // initialized found BogWordList

    bool expandFound(); // expands size of found's words array

    int numQusInWord(const BogWord); // returns number of Qs in word
   
    // currentWord modifying and operation functions
    
    void addToCurrWord(BogLett); // adds letter to currWord

    void removeFromCurrWord(); // "removes" letter from currWord

    void addCurrToFound(); // copies and adds current word to found list

    bool currWordIsPrefix(); // checks if current word is valid prefix

    bool currWordIsWord(); // checks if current word is valid word

    string currWordToString(); // converts current word to string

    // conversion functions

    int indexToX(int); // converts board index to x coordinate (cols)

    int indexToY(int); // converts board index to y coordinate (rows)
};

#endif
