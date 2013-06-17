Created by    : Teddy Cleveland
Last Modified : 05.02.2013

Project 2 Part 2:

Boggle Solver, Checker, and Scorer

Compile Commands for Programs are as follows:

bbsolver:: g++ -Wall -Wextra solverMain.cpp BogSolver.cpp Dictionary.cpp Matrix.cpp -o bbsolver
bbchecker: g++ -Wall -Wextra checkerMain.cpp BogValidator.cpp Dictionary.cpp Matrix.cpp -o bbchecker
bbscorer: g++ -Wall -Wextra scorerMain.cpp BogScorer.cpp -o bbscorer

The implementation uses a graph to represent the board, with Matrix.h
generating an adjacenecy matrix showing each piece's connecetions to 
those around it. This makes navigating the board through recursion
relatively clean. The actual board is represented in an array of 
boardNodes containing a "used" bool and the BogLett in that space.
The used value is used to ensure that words do not reuse letters 
on the board, and the adjacency matrix lets the program know which
indecies link with the others. 

Thanks to the COMP15 Team! It's been an awesome semester. 



