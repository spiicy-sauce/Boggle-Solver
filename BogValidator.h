#ifndef BOGVALIDATOR
#define BOGVALIDATOR

#include <iostream>
#include "Dictionary.h"
#include "BogWordList.h"
#include "Matrix.h"

using namespace std;


class BogValidator
{
public:

    BogValidator(); // constructor

    ~BogValidator(); // destructor

    bool readDict(); // read in a dictionary

    bool readBoard(); // read in a board

    bool isValid(string s); // validates one word

    void checkWords(); // validates cin

private:

    Dictionary dict; // 

    Dictionary usedDict; // previously used words

    BoardNode* board; // board letters

    Matrix* MatrixGenerator; // creates matrix

    bool** bogMatrix; // links from each piece to those around it

    BogWord currWord; // current word being searched

    int height, width, maxWordSize; // size information

    string currentSearch;

    bool wordFound;

    void makeMatrix();

    void checkVertextForWord(int i);

    bool isPrefixToSearch();

    bool stringEqualsCurrent(string);

    string currWordToString();

    void addToCurrWord(BogLett);

    void removeFromCurrWord();

    int indexToX(int index);

    int indexToY(int index);

};

#endif
