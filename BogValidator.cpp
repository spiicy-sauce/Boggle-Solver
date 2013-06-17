
#include "BogValidator.h"
#include <cstring>

/* Defines     : BogValidator.h
 * Represents  : A word validating system for a Boggle board
 * Interactions: uses a Matrix.h and Dictionary.h classes. Matrix.h
                 creates an adjacency list that allows the validator
                 to navigate the board. Dictionary stores word. 
 * Created By  : Teddy Cleveland
 * Last Mod    : 05/02/2013 */

///////////////// BEGIN FUNCTION DEFINITIONS ///////////////////////
 

/* Does      : constructor */

BogValidator::BogValidator()
{
    wordFound       = false;
    board           = NULL;
    bogMatrix       = NULL;
    MatrixGenerator = NULL;
    height          = 0;
    width           = 0;
    maxWordSize     = 0;
}

/*  Does      : destructor */
BogValidator::~BogValidator()
{
    delete board;
    delete MatrixGenerator;
    delete currWord.letts;
}

/*  Does   : Reads in dictionary
 *  Returns: true if dict reads everything in
 *  Calls  : dict.insert() */

bool BogValidator::readDict()
{
    string word;
    cin >> word;
    while(word != ".")
    {
        if(!dict.insert(word)){return false;}
        cin >> word;
    }
    return true;
}


/*  Does   : Reads in board and generates adjacency matrix
 *  Returns: bool if board is successfully created
 *  Calls  : makeBogmatrix()
 *  Notes  : Implementation of board is as graph. Board
        consists of array of BoardNodes. Each contains
        BogLett and "used" bool for tracking blocks in
        use. */

bool BogValidator::readBoard()
{
    int rows, cols;
    char lett;

    cin >> rows >> cols;

    height       = rows;
    width        = cols;
    maxWordSize  = rows * cols;

    board = new BoardNode[height*width];
    
    if(board == NULL) {return false;}

    for(int i = 0; i < (height*width); i++)
    {
        cin >> lett;
        board[i].lett.c   = lett;
        board[i].lett.col = indexToX(i);
        board[i].lett.row = indexToY(i);
        board[i].used     = false;
    }

    makeMatrix();

    return true;
}

/*  Does      : checks for all input words on the board
                outputs result
 *  Calls     : isValid(string) usedDict.insert(string)
 *  Notes     : the main workhorse function for this function
                is isValid*/

void BogValidator::checkWords()
{
    currWord.numLetts = 0;
    currWord.letts = new BogLett[maxWordSize];

    string word;

    while(cin >> word)
    {
        currWord.numLetts = 0;

        if(isValid(word))
        {
            usedDict.insert(word);
            cout << "OK " << word << endl; 
        }
        else 
        {
            cout << "NO " << word << endl;
        }
    }
}

/*  Does      : checks if a single word is on the board
 *  Arguments : word to check
 *  Returns   : true if word is found
 *  Calls     : dict.isWord(string), addToCurrWord(BogLett),
                checkVertexForWord(int), removeFromCurrWord()
 *  Notes     : This functions main workhorse is checkVertexForWord
                each time it finds a possible path (a letter that matches
                the first letter of the search word). If checkVertexForWord
                finds the word on the board, it sets wordFound to true, 
                and isValid returns true. If all of the letters that are
                the same as the first letter of the search word have been
                inspected and the word is not found, isValid returns false*/

bool BogValidator::isValid(string s)
{   
    if(!dict.isWord(s) || s.length() < 3 || usedDict.isWord(s)) {return false;}

    currentSearch = s;

    for(int i = 0; i < height * width; i++)
    {
        if(board[i].lett.c == s[0])
        {
            board[i].used = true;
            addToCurrWord(board[i].lett);

            checkVertextForWord(i);

            board[i].used = false;
            removeFromCurrWord();

            if(wordFound) 
            {
                wordFound = false;
                return true;
            }
        }
    }
    return false;
}

/*  Does      : Beginning at an input vertex, checks all possible paths aroudn
                vertex for word being searched
 *  Arguments : root vertex
 *  Returns   : nothing, but sets wordFound to true if it finds word
 *  Calls     : currWordToString(), addToCurrWord(BogLett), isPrefixToSearch(),
                checkVertexForWord(int) (recursive), removeFromCurrWord()
 *  Notes     : This function works recursively to explore all paths around
                input vertex to build up word possibilities. If at any point,
                the building word is not a prefix to the word being searched,
                the path is terminated and other are tried. If the word is not
                found stemming from the input root, wordFound remains false */

void BogValidator::checkVertextForWord(int root)
{
    if(currWordToString() == currentSearch)
    {
        wordFound = true;
        return;
    }

    for(int i = 0; i < (height * width); i++)
    {
        if((bogMatrix[root][i]) && (!board[i].used))
        {
            
            addToCurrWord(board[i].lett);
            board[i].used = true;
            if(isPrefixToSearch())
            {
                checkVertextForWord(i);
            }
            board[i].used = false;
            removeFromCurrWord();
        }
    }
}

/*  Does      : checks if currWord is a prefix to currentSearch
 *  Returns   : true if currWord is prefix to search word
 *  Calls     : currWordToString()
 *  Notes     : used by checkVertexForWord to know if path cannot possibly
                contain word */

bool BogValidator::isPrefixToSearch()
{

    string s = currWordToString();

    for(int i = 0; i < (int)s.length(); i++)
    {
        if(s[i] != currentSearch[i])
        {
            return false;
        }
    }
    return true;
}
/* Does       : makes matrix for board. See Matrix.h and Matrix.cpp for 
                notes */

void BogValidator::makeMatrix()
{
    MatrixGenerator = new Matrix(height, width);
    bogMatrix = MatrixGenerator -> makeBogMatrix();
}

/*  Does      : "deletes" a letter from the end of currWord
 *  Notes     : this is a "lazy" delete the next letter will now 
                overwrite the one that was "deleted" */

void BogValidator::removeFromCurrWord()
{
    currWord.numLetts -= 1;
}


/*  Does      : Adds a letter to end of CurrWord
 *  Arguments : BogLett to add
 *  Notes     : Just inserted at numLetts position of
                currword. Overwrites any previous data.
                increments numLetts. */

void BogValidator::addToCurrWord(BogLett l)
{
    currWord.letts[currWord.numLetts] = l;
    currWord.numLetts += 1;
}


/*  Does      : converts currWord to a string
 *  Returns   : string value of currWord */

string BogValidator::currWordToString()
{
    string s;
    for(int i = 0; i < currWord.numLetts; i++)
    {
        if(currWord.letts[i].c == 'Q')
        {
            s += "QU";
        }
        else
        {
            s += currWord.letts[i].c;
        }
    }
    return s;
}

/*  Does: converts index to x coordinate
 *  Arguments: index
 *  Returns: x coordinate 
 *  Calls: none
 *  Notes: because the board is stored in a
        single array, this conversion is 
        needed for a 2D representation */

int BogValidator::indexToX(int index)
{
    return index % width;
}

/*  Does: converts index to y coordinate
 *  Arguments: index
 *  Returns: y coordinate
 *  Calls: indexToX(int)
 *  Notes: see indexToXNotes */


int BogValidator::indexToY(int index)
{
    return (index - indexToX(index)) / width;
}
