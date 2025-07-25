#ifndef SUDOKU_HPP
#define SUDOKU_HPP




#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;


class sudoku {
    
public:
    struct cell {
        int num, pos, col, row, box;
        int notes[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    };

    cell grid[81];
    vector<cell*> filledEntries; //a list of pointers to completed cells

    sudoku();

    //finders
    int getCol(int pos);
    int getRow(int pos);
    int getBox(int pos);
    int getBoxHead(int boxNum);

    //seekers
    bool inCol(int num, int pos);
    bool inRow(int num, int pos);
    bool inBox(int num, int pos);
    bool valid(int num, int pos);

    //Accessors
    cell* accessCol(int col, int cellNum);
    cell* accessRow(int row, int cellNum);
    cell* accessBox(int box, int cellNum);
    

    //evaluation
    void doublesEqual(cell* cellA, cell* cellB);
    int* doublesCompare(cell cellA, cell cellB);
    bool checkPointingPair(cell* cellA, cell* cellB);

    bool complete();
    void place(int num, int pos);

    //Solving Methods
    void noteScan();
    void nakedSingles();
    void hiddenSingles();
    void nakedDoubles();    //uses doublesNoteClean
    void pointingPair();    //uses doublesNoteClean
    void doublesNoteClean(cell* cellA, cell* cellB, int noteA, int noteB);
    void methodCycle();                                                    /*Call this every time you want the sudoku to update with new cells
                                                                             This will start all solving from the beginning again*/

    //debug
    void displayGrid();
    void peek(int pos);    //lists the notes at a cell
    void info(int pos);    //prints all relevant info for a cell (number, row, col, box, and notes)
};

#endif