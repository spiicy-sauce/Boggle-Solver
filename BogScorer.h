
#ifndef BOGSCORER
#define BOGSCORER

#include <iostream>
#include <cstring>

using namespace std;

class BogScorer
{

public:

    BogScorer(); // constructor

    void scoreWords(); // scores and prints words

    void printReport(); // prints final values

private:

    int score(string); // returns score of single string

    int totalWords; // total valid words

    int totalPoints; // total points earned

};

#endif
