//BogWordList.h
//Defines structs for wordlist
//Do not modify these structs 

#ifndef BOGWORDLIST
#define BOGWORDLIST

struct BogLett {
    char c;
    int row, col;
};

struct BogWord {
    int numLetts;
    BogLett* letts;
};

struct BogWordList {
    int numWords;
    BogWord* words;
};

struct BoardNode {
    BogLett lett;
    bool used;
};

#endif
