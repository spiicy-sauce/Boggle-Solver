
#include "Matrix.h"

/* Defines: Matrix.h
 * Represents: A matrix generating set of functions
 * Interactions: used by BogSolver and BogChecker
 * Created By  : Teddy Cleveland
 * Last Mod    : 05/02/2013 */

/////////////////// BEGIN FUNCTION DEFINITIONS /////////////////


/*  Does      : constructor
 *  Notes     : automatically sets everything to NULL or zero*/

Matrix::Matrix() 
{
    BogMatrix = NULL;
    height = 0;
    width = 0;
    size = 0;
}

/*  Does      : overloaded constructor
 *  Arguments : rows and cols
 *  Notes     : sets values of all private data members*/

Matrix::Matrix(int rows, int cols)
{
    height = rows;
    width = cols;
    size = rows * cols;
    BogMatrix = new bool*[size];
    for(int i = 0; i < size; i++)
    {
        BogMatrix[i] = new bool[size];
        for(int j = 0; j < size; j++)
        {
            BogMatrix[i][j] = false;

        }
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i < size; i++)
    {
        delete BogMatrix[i];
    }
    delete BogMatrix;
}


/*  Does      : Generates a matrix based on input specifications (from 
                overloaded constructor)
 *  Returns   : pointer to the newly generated matrix
 *  Calls     : linkToSurroundings(int)
 *  Notes     : goes to each piece and links it to its surroundings */

bool** Matrix::makeBogMatrix()
{
    for(int i = 0; i < size; i++)
    {
        linkToSurroundings(i);
    }
    return BogMatrix;
}

/*  Does      : links an index i to those it would be adjacent to
                in a 2D representation
 *  Arguments : index to link
 *  Calls     : all edge evaluation functions, all adjacent piece finding
                functions. */

void Matrix::linkToSurroundings(int i)
{
    if(!isTop(i)){
        if(!isLeft(i)) 
        {
            BogMatrix[i][L(i)] = true;
            BogMatrix[i][TL(i)] = true;
        }

        if(!isRight(i))
        {
            BogMatrix[i][R(i)] = true;
            BogMatrix[i][TR(i)] = true;

        }
    
        BogMatrix[i][T(i)] = true;
    }

    if(!isBot(i))
    {
        if(!isLeft(i)) 
        {
            BogMatrix[i][L(i)] = true;
            BogMatrix[i][BL(i)] = true;
        }

        if(!isRight(i))
        {
            BogMatrix[i][R(i)] = true;
            BogMatrix[i][BR(i)] = true;
        }
        
        BogMatrix[i][B(i)] = true;
    }

    if(isTop(i) && isBot(i))
    {
        if(!isLeft(i))
        {
            BogMatrix[i][L(i)] = true;
        }
        if(!isRight(i))
        {
            BogMatrix[i][R(i)] = true;
        }
    }

    return;
}


// edge evaluation fucntion definitions

bool Matrix::isTop(int i) 
{
    return i < width;
}

bool Matrix::isRight(int i) 
{
    return (i % width) == (width - 1);
}

bool Matrix::isLeft(int i) 
{
    return i % width == 0;
}

bool Matrix::isBot(int i) 
{
    return i >= (height - 1) * width;
}

// adjacent piece finder function definitions

int Matrix::T(int i)  {return i - width;} // top

int Matrix::R(int i)  {return i + 1;}     // right

int Matrix::L(int i)  {return i - 1;}     // left

int Matrix::B(int i)  {return i + width;} // bottom
 
int Matrix::TR(int i)  {return T(R(i));}  // top-right

int Matrix::BR(int i)  {return B(R(i));}  // bottom-right

int Matrix::BL(int i)  {return B(L(i));}  // bottom-left

int Matrix::TL(int i)  {return T(L(i));}  // top-left


