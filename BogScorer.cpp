
#include "BogScorer.h"

const string VALID = "OK";

/* Defines     : BogScorer.h
 * Represents  : A word scoring system for a Boggle board
 * Interactions: Scores output from BogValidator
 * Created By  : Teddy Cleveland
 * Last Mod    : 05/02/2013 */


//////////////////// BEGIN CLASS FUNCTION DEFINITIONS ////////////////////////

/*  Does      : Constructor, initializes score and word count to zero */

BogScorer::BogScorer()
{
    totalPoints = 0;
    totalWords  = 0;
}


/*  Does      : Scores input words
 *  Calls     : score(string)
 *  Notes     : prints result depending on validity of word */

void BogScorer::scoreWords()
{
    string isValid, word;

    while(cin >> isValid)
    {
        cin >> word;
        if(isValid == VALID)
        {
            cout << score(word) << " " << isValid << " " << word << endl;
            totalPoints += score(word);
            totalWords++;
        }
        else 
        {
            cout << 0 << " " << isValid << " " << word << endl;
        }

    }
}


/*  Does      : scores single word based on length
 *  Arguments : word to score
 *  Returns   : integer score
 *  Calls     : string.length()
 *  Notes     : based on boggle Parker Brothers Boggle® Rules */

int BogScorer::score(string s)
{
    if(s.length() == 3 || s.length() == 4)
    {
        return 1;
    }
    else if (s.length() == 5)
    {
        return 2;
    }
    else if(s.length() == 6)
    {
        return 3;
    }
    else if(s.length() == 7)
    {
        return 5;
    }
    else
    {
        return 11;
    }
}


/*  Does      : prints final word and score counts */

void BogScorer::printReport()
{
    cout << totalWords << " words " << totalPoints << " points\n";
}
