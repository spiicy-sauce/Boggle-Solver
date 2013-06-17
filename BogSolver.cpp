
#include "BogSolver.h"



/* Defines     : BogSolver.h
 * Represents  : A word finding system for a Boggle board
 * Interactions: uses a Matrix.h and Dictionary.h classes. Matrix.h
                 creates an adjacency list that allows the solver
                 to navigate the board. Dictionary stores word. 
 * Created By  : Teddy Cleveland
 * Last Mod    : 05/02/2013 */


//////////// BEGIN PUBLIC MEMBER FUNCTION DEFINITIONS ////////////////////

/*  Does   : Constructor. Sets initial values */

BogSolver::BogSolver()
{
    board           = NULL;
    bogMatrix       = NULL;
    MatrixGenerator = NULL;
    height          = 0;
    width           = 0;
    maxWordSize     = 0;
    foundCap        = 0;
}

/* Does    : Destructor. Frees all allocated memory*/

BogSolver::~BogSolver()
{

    for(int i = 0; i < found.numWords; i++)
    {
        delete found.words[i].letts;
    }

    delete board;
    delete found.words;
    delete currWord.letts;
    delete MatrixGenerator;

}

/*  Does   : Reads in dictionary
 *  Returns: true if dict reads everything in
 *  Calls  : dict.insert() */

bool BogSolver::readDict()
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

bool BogSolver::readBoard()
{
    int rows, cols;
    char letter;

    cin >> rows >> cols;

    height       = rows;
    width        = cols;
    maxWordSize  = rows * cols;

    board = new BoardNode[height*width];
    
    if(board == NULL) {return false;}

    for(int i = 0; i < (height*width); i++)
    {
        cin >> letter;
        board[i].lett.c   = toupper(letter);
        board[i].lett.col = indexToX(i);
        board[i].lett.row = indexToY(i);
        board[i].used     = false;
    }


    makeMatrix();

    return true;
}


/*  Does      : initializes found list and currWord.
                finds all words on board.
 *  Returns   : boolean if words have been found
 *  Calls     : addToCurrWord(BogLett), wordSearch(int),
                removeFromCurrWord()
 *  Notes     : passes each letter as a "root" vertex to
                wordSearch, which finds all words stemming
                from that vertex and stores them in the 
                found wordlist. */

bool BogSolver::solve()
{
    currWord.letts = new BogLett[maxWordSize];
    currWord.numLetts = 0;

    if(!initFound()) {return false;}

    for(int i = 0; i < height * width; i++)
    {
        addToCurrWord(board[i].lett);
        board[i].used = true;
        wordSearch(i);
        board[i].used = false;
        removeFromCurrWord();

    }

    return true;
}


/*  Does      : returns the number of words found */

int BogSolver::numWords()
{
    return found.numWords;
}


/*  Does      : returns the number of words of length len
 *  Arguments : integer length
 *  Returns   : integer number of words of length len
 *  Calls     : isQuInWord(BogWord), numQusInWord(BogWord)
 *  Notes     : for each Q in a word, the function adds another
                letter to the length count before comparing
                to the input length. */

int BogSolver::numWords(int len)
{
    int wordCount = 0;
    for(int i = 0; i < found.numWords; i++)
    {
        // if Q's in word, adds 2 to numletts for every Q
        if(numQusInWord(found.words[i]) +             \
           found.words[i].numLetts == len)
        {
            wordCount++;
        }   
    }
    return wordCount;
}


/*  Does      : returns a copy of the found word list
 *  Returns   : pointer to new BogWordList copy of found
 *  Notes     : makes deep copy so that user does not have
                access to private found wordList  */

BogWordList* BogSolver::getWords()
{
    BogWordList* allWords  = new BogWordList; 
    allWords -> numWords   = found.numWords;
    allWords -> words      = new BogWord[found.numWords];

    for(int i = 0; i < found.numWords; i++)
    {
        allWords -> words[i].numLetts = found.words[i].numLetts;
        allWords -> words[i].letts = new BogLett[found.words[i].numLetts];

        for(int j = 0; j < allWords -> words[i].numLetts; j++)
        {
            allWords -> words[i].letts[j] = found.words[i].letts[j];
        }
    }

    return allWords;
}


/*  Does      : returns a copy of all found words of 
                length len
 *  Arguments : length of words to be returned
 *  Returns   : pointer to new BogWordList
 *  Calls     : numWords(int), numQusInWord(BogWord)
 *  Notes     : creates a new list with enough spaces to fit
                all words of length len */

BogWordList* BogSolver::getWords(int len)
{
    BogWordList* lenWords = new BogWordList;
    lenWords -> numWords  = numWords(len);
    lenWords -> words     = new BogWord[numWords(len)];
    int wordIndex = 0;

    for(int i = 0; i < found.numWords; i++)
    {
        // if Q's in word, adds 1 to numletts for every Q
        if(numQusInWord(found.words[i]) +               \
           found.words[i].numLetts == len)
        {
            // assigns numLetts for each word
            lenWords->words[wordIndex].numLetts =       \
                found.words[i].numLetts;

            // creates new BogLett array for each word
            lenWords->words[wordIndex].letts    =       \
                new BogLett[found.words[i].numLetts];

            // coppies letters into each new word
            for(int j = 0; j < found.words[i].numLetts; j++)
            {
                lenWords -> words[wordIndex].letts[j] = \
                    found.words[i].letts[j];
            }
            
            wordIndex++;
        }  
    }
    return lenWords;
}

/*  Does      : prints all words in HBF format
 *  Notes     : converts Q to QU in printing
 */

void BogSolver::printWords()
{
    for(int i = 0; i < found.numWords; i++)
    {
        cout << "< ";
        for(int j = 0; j < found.words[i].numLetts; j++)
        {
            if(found.words[i].letts[j].c == 'Q')
            {
                 cout << "QU "; 
            }
            else 
            {
                cout << found.words[i].letts[j].c << " ";
            }
            cout << found.words[i].letts[j].row << " " \
                 << found.words[i].letts[j].col << " ";
        }
        cout << ">" << endl;
    }
    cout << "< >" << endl;
}

/*  Does      : prints all words of length len in HBF format
 *  Arguments : length of words to print
 *  Calls     : numQusInWord(BogWord);
 *  Notes     : Only prints word if the number of Q's in the word + 
                the word's length == the input length. */

void BogSolver::printWords(int len)
{
    for(int i = 0; i < found.numWords; i++)
    {
        if(found.words[i].numLetts +               \
                 numQusInWord(found.words[i]) == len)
        {
            cout << "< ";
            for(int j = 0; j < found.words[i].numLetts; j++)
            {
                if(found.words[i].letts[j].c == 'Q')
                {
                     cout << "QU "; 
                }
                else 
                {
                    cout << found.words[i].letts[j].c << " ";
                }
                cout << found.words[i].letts[j].row << " " \
                     << found.words[i].letts[j].col << " ";
            }
            cout << " >" << endl;
        }
    }
    cout << "< >" << endl;
}


/*  Does      : lists all words
 *  Notes     : converts Q's to QU */

void BogSolver::listWords()
{
    for(int i = 0; i < found.numWords; i++)
    {
        for(int j = 0; j < found.words[i].numLetts; j++)
        {
            if(found.words[i].letts[j].c == 'Q')
            {
                cout << "QU";
            }
            else
            {
                cout << found.words[i].letts[j].c;
            }
        }
        cout << endl;
    }
}


/*  Does      : lists words of length len
 *  Arguments : length of words to list
 *  Calls     : numQusInWord(BogWord)
 *  Notes     : only prints word if the word's lenght + the number of
                Q's in the word equals the input length */

void BogSolver::listWords(int len)
{
    for(int i = 0; i < found.numWords; i++)
    {
        if (found.words[i].numLetts + numQusInWord(found.words[i]) == len)
        {
            for(int j = 0; j< found.words[i].numLetts; j++)
            {
                if(found.words[i].letts[j].c == 'Q')
                {
                    cout << "QU";
                }
                else 
                {
                    cout << found.words[i].letts[j].c;
                }
            }
            cout << endl;
        }
    }
}

//////////// END PUBLIC MEMBER FUNCTION DEFINITIONS ////////////////////

//////////// BEGIN PRIVATE MEMBER FUNCTION DEFINITIONS ///////////////////


//////////// BEGIN HELPER FUNCTION DEFINITIONS /////////////////


/* Does       : makes matrix for board. See Matrix.h and Matrix.cpp for 
                notes */

void BogSolver::makeMatrix()
{
    MatrixGenerator = new Matrix(height, width);
    bogMatrix = MatrixGenerator -> makeBogMatrix();
}



/*  Does      : recursively searches the board for words starting
                from the index passed in by parent function. This is the main
                search function.
 *  Arguments : vertex to begin from
 *  Calls     : currWordIsWord(), addCurrToFound(BogLett), currWordIsPrefix(),
                wordSearch(int) (recursive), removeFromCurrWord()
 *  Notes     : the function works recursively, inspecting each connection 
                from the root to the vertecies around it. If a link presents
                a possible word (is a prefix to a word in the dictionary),
                the second vertex is added to the currWord and its marker
                is set to used so that it is not included in the word again.
                once all possible paths have been explored through that edge,
                the vertex is reset to unused, the letter is removed, and the 
                loop continues with the rest of the links. */

void BogSolver::wordSearch(int root)
{
    if(currWordIsWord())
    {
        addCurrToFound();
    }

    for(int i = 0; i < (height * width); i++)
    {
        if((bogMatrix[root][i]) && (!board[i].used))
        {
            
            addToCurrWord(board[i].lett);
            board[i].used = true;
            if(currWordIsPrefix())
            {
                wordSearch(i);
            }
            board[i].used = false;
            removeFromCurrWord();
        }
    }
}  


/*  Does      : initializes the found wordlist
 *  Returns   : true if space was allocated */

bool BogSolver::initFound()
{

       found.words = new BogWord[0];
    if(found.words == NULL) {return false;}

    found.numWords = 0;
    return true;
}


/*  Does      : expands the size of the found worlist
 *  Returns   : true if space was allocated
 *  Notes     : expansion by a multiple of 2 */

bool BogSolver::expandFound()
{
    BogWord* oldWords = found.words;

    found.words = new BogWord[foundCap * 2 + 1];
    if(found.words == NULL) {return false;}

    for(int i = 0; i < foundCap ; i++)
    {
        found.words[i].numLetts = oldWords[i].numLetts;
        found.words[i].letts = new BogLett[oldWords[i].numLetts];

        for(int j = 0; j < oldWords[i].numLetts; j++)
        {
            found.words[i].letts[j] = oldWords[i].letts[j];
        }

        delete oldWords[i].letts;
    }
    delete oldWords;

    foundCap = foundCap * 2 + 1;
    return true;

}


/*  Does      : calculates the number of Q's in a word
 *  Arguments : const BogWord to check
 *  Returns   : numver of Q's in word
 *  Notes     : used for functions that are searching for
                words of a specific length. This way they
                can add 1 to the word's length for every 
                Q in the word (because Q == QU on board. */

int BogSolver::numQusInWord(const BogWord w)
{
    int n = 0;
    for(int i = 0; i < w.numLetts; i++)
    {
        if(w.letts[i].c == 'Q')
        {
            n++;
        }
    }
    return n;
}


//////////// END HELPER FUNCTION DEFINITIONS /////////////////


//////////// BEGIN CURRENTWORD MODIFYING FUNCTION DEFINITIONS /////////////////


/*  Does      : Adds a letter to end of CurrWord
 *  Arguments : BogLett to add
 *  Notes     : Just inserted at numLetts position of
                currword. Overwrites any previous data.
                increments numLetts. */

void BogSolver::addToCurrWord(BogLett l)
{
    currWord.letts[currWord.numLetts] = l;
    currWord.numLetts += 1;
}


/*  Does      : "deletes" a letter from the end of currWord
 *  Notes     : this is a "lazy" delete the next letter will now 
                overwrite the one that was "deleted" */

void BogSolver::removeFromCurrWord()
{
    currWord.numLetts -= 1;
}


/*  Does      : adds current BogWord to the list of found words
 *  Calls     : expandFound(), dict.insert, currWordToString()
 *  Notes     : takes the word that is currently stored in currWord
                and inserts it into the found BogWordList. If the
                number of words in the list are equal to the cap,
                the function expands the found list. the word
                is then converted to a string and inserted into
                the found dictionary, and the number of found
                words is incremented by one. */

void BogSolver::addCurrToFound()
{
    if(found.numWords == foundCap) {expandFound();}

    int currEmpty = found.numWords;

    // insertion of new word of lenght currWord.numLetts
    found.words[currEmpty].numLetts = currWord.numLetts;
    found.words[currEmpty].letts    = new BogLett[currWord.numLetts];

    // deep copy of each letter,
    for(int i = 0; i < currWord.numLetts; i++)
    {
        found.words[currEmpty].letts[i] = currWord.letts[i];
    }

    foundDict.insert(currWordToString());
    found.numWords += 1;
}


/*  Does      : checks if currWord is a prefix to a word in dict
 *  Returns   : true if word is prefix to dict word
 *  Calls     : currWordToString()
 *  Notes     : the use of this function eliminates the majority
                of possible paths on the board. if this returns
                false at any given call, the search function will
                stop searching its current path because the curr
                Word can not possibly result in a valid word from
                that point.*/

bool BogSolver::currWordIsPrefix()
{
    return dict.isPrefix(currWordToString());
}


/*  Does      : checks if the current word is valid
 *  Returns   : true if word is valid (more than three chars,
                in dict, and not already found)
 *  Calls     : dict.isWord(), currWordToString()
 *  Notes     : used to evaluate if the current path of currWord is
                an acceptable word */

bool BogSolver::currWordIsWord()
{
    return 
        currWordToString().length() > 2    && \
        dict.isWord(currWordToString())    && \
        !foundDict.isWord(currWordToString()) ;
}


/*  Does      : converts currWord to a string
 *  Returns   : string value of currWord */

string BogSolver::currWordToString()
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

//////////// END CURRENTWORD MODIFYING FUNCTION DEFINITIONS /////////////////


/////////// COORDINATE CONVERSION FUNCTION DEFINITIONS //////////////

/*  Does      : converts index to x coordinate
 *  Arguments : index
 *  Returns   : x coordinate 
 *  Calls     : none
 *  Notes     : because the board is stored in a
        single array, this conversion is 
        needed for a 2D representation */

int BogSolver::indexToX(int index)
{
    return index % width;
}

/*  Does      : converts index to y coordinate
 *  Arguments : index
 *  Returns   : y coordinate
 *  Calls     : indexToX(int)
 *  Notes     : see indexToXNotes */


int BogSolver::indexToY(int index)
{
    return (index - indexToX(index)) / width;
}

//////////// END PRIVATE MEMBER FUNCTION DEFINITIONS ///////////////////
