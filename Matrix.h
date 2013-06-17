#ifndef MTRX
#define MTRX

#include <iostream>
using namespace std;

class Matrix 
{

public:

    Matrix(); // constructor
    Matrix(int, int); // overloaded constructor takes height and width
    ~Matrix(); // destructor
    bool** makeBogMatrix(); // creates boggle board matrix

private:

    bool** BogMatrix;
    int height, width, size;

    void linkToSurroundings(int); // links vertex with surrounding verticies

    // functions that check if piece is edge
    bool isTop(int);
    bool isRight(int);
    bool isLeft(int);
    bool isBot(int);

    // functions that return adjacent piece
    // indecies 
    int T(int);  // top
    int TR(int); // top-right
    int R(int);  // right
    int BR(int); // bottom-right
    int B(int);  // bottom
    int BL(int); // bottom-left
    int L(int);  // left
    int TL(int); // top-left
};

#endif
