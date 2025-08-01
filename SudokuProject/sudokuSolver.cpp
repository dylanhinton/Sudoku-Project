
#include "sudokuSolver.hpp"

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <array>
#include <cstdlib>

using namespace std;


sudoku::sudoku() {

    int gridSample[81] = {
        0, 8, 0, 6, 0, 0, 0, 2, 0,
        0, 0, 0, 0, 9, 5, 0, 0, 7,
        4, 0, 5, 0, 3, 2, 0, 0, 0,
        0, 6, 0, 1, 5, 0, 0, 0, 4,
        0, 0, 9, 0, 0, 0, 0, 5, 0,
        0, 2, 0, 0, 0, 0, 8, 0, 0,
        0, 0, 0, 0, 0, 0, 7, 0, 6,
        9, 0, 0, 0, 6, 0, 0, 0, 0,
        6, 0, 0, 0, 4, 1, 9, 0, 0
    };
    /* Empty Grid
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0
    */

    for (int i = 0; i < 81; i++) {
        //grid[i].num = 0;
        grid[i].num = gridSample[i];
        if (grid[i].num != 0) {
            filledEntries.push_back(&grid[i]);
            for (int x = 0; x < 9; x++) {       //ensures that the only note in a filled out position is the number of said position
                if (x != grid[i].num - 1) {
                    grid[i].notes[x] = 0;
                }
            }
        }
        grid[i].pos = i;
        grid[i].col = getCol(i);
        grid[i].row = getRow(i);
        grid[i].box = getBox(i);
    }
    


};

//Finders

int sudoku::getCol(int pos) {
    return pos % 9;
};


int sudoku::getRow(int pos) {
    return pos / 9;
};


int sudoku::getBox(int pos) {
    int col = getCol(pos) / 3;
    int row = getRow(pos) / 3;
    return col + (3 * row);
}


int sudoku::getBoxHead(int boxNum) {
    int col = (boxNum % 3) * 3;
    int row = (boxNum / 3) * 3;
    int boxHead = (row * 9) + col;
    return boxHead;
}


//Seekers

bool sudoku::inCol(int num, int pos) {
    int col = grid[pos].col;
    for (int i = 0; i < 9; i++) {
        if (num == grid[(i * 9) + col].num) {
            return true;
        }
    }
    return false;
};


bool sudoku::inRow(int num, int pos) {
    int row = grid[pos].row;
    for (int i = 0; i < 9; i++) {
        if (num == grid[(row * 9) + i].num) {
            return true;
        }
    }
    return false;
};


bool sudoku::inBox(int num, int pos) {
    int box = grid[pos].box;
    int col = 3 * (box % 3);
    int row = 3 * (box / 3);
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (num == grid[(9 * (row + y)) + (col + x)].num) {
                return true;
            }
        }
    }
    return false;
};


bool sudoku::contains(sudoku::cell* cell, int num) {
    if (cell->notes[num]) {
        return true;
    }
    return false;
}


bool sudoku::valid(int num, int pos) {
    if (inCol(num, pos) || inRow(num, pos) || inBox(num, pos)) {
        return false;
    }
    else {
        return true;
    }
};


bool sudoku::complete() {
    for (int i = 0; i < 81; i++) {
        if (grid[i].num == 0) {
            return false;
        }
    }
    return true;
};


//Accessors

sudoku::cell* sudoku::accessCol(int col, int cellNum) {
    if (col >= 9 || col < 0) {
        cout << "ERROR: Access Violation\n" << col << " out of bounds" << endl;
        return 0;
    }
    if (cellNum >= 9 || cellNum < 0) {
        cout << "ERROR: Access Violation\n" << cellNum << " out of bounds" << endl;
        return 0;
    }
    cell* colCell = &grid[col + (9 * cellNum)];
    return colCell; 
};


sudoku::cell* sudoku::accessRow(int row, int cellNum) {
    if (row >= 9 || row < 0) {
        cout << "ERROR: Access Violation\n" << row << " out of bounds" << endl;
        return 0;
    }
    if (cellNum >= 9 || cellNum < 0) {
        cout << "ERROR: Access Violation\n" << cellNum << " out of bounds" << endl;
        return 0;
    }
    cell* rowCell = &grid[cellNum + (9 * row)];
    return rowCell;
};


sudoku::cell* sudoku::accessBox(int box, int cellNum) {
    if (box >= 9 || box < 0) {
        cout << "ERROR: Access Violation\n" << box << " out of bounds" << endl;
        return 0;
    }
    if (cellNum >= 9 || cellNum < 0) {
        cout << "ERROR: Access Violation\n" << cellNum << " out of bounds" << endl;
        return 0;
    }
    int boxHead = getBoxHead(box);
    int col = cellNum % 3;
    int row = (cellNum / 3) * 9;
    cell* boxCell = &grid[boxHead + col + row];
    return boxCell;
};

sudoku::cell* sudoku::accessGrid(int pos) {
    if (0 > pos || pos >= 81) {
        cout << "ERROR: Access Violation\n" << pos << " out of bounds" << endl;
        return 0;
    }
    cell* gridCell = &grid[pos];
    return gridCell;
}

void sudoku::place(int num, int pos) {      //This goes under some other category
    if (pos < 0 || pos >= 81) {
        cout << "ERROR: Access Violation\n" << pos << " Out of bounds" << endl;
        return;
    }
    grid[pos].num = num;
    filledEntries.push_back(&grid[pos]);
    for (int i = 0; i < 9; i++) {       //ensures that the only note in a filled out position is the number of said position
        if (i != num - 1) {
            grid[pos].notes[i] = 0;
        }
    }
};

void sudoku::unplace() {
    filledEntries.back()->num = 0;
    filledEntries.pop_back();
}

//Utility

array<int, 9> sudoku::columnSum(int col) {
    array<int, 9> noteSum = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 9; i++) {
        for (int n = 0; n < 9; n++) {
            noteSum[n] += accessCol(col, i)->notes[n];
        }
    }
    return noteSum;
};


array<int, 9> sudoku::rowSum(int row) {
    array<int, 9> noteSum = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 9; i++) {
        for (int n = 0; n < 9; n++) {
            noteSum[n] += accessRow(row, i)->notes[n];
        }
    }
    return noteSum;
};


array<int, 9> sudoku::boxSum(int box) {
    array<int, 9> noteSum = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 9; i++) {
        for (int n = 0; n < 9; n++) {
            noteSum[n] += accessBox(box, i)->notes[n];
        }
    }
    return noteSum;
};


//Evaluation

void sudoku::doublesEqual(cell* cellA, cell* cellB) {
    int totalEquals = 0;                    //Tracks equal non-zero values.
    for (int i = 0; i < 9; i++) {
        if (cellA->notes[i] != cellB->notes[i]) {
            return;
        }
        else if (cellA->notes[i] != 0) {
            totalEquals++;
        }
    }
    if (totalEquals == 2) {
        int notes[2] = {0,0};
        int point = 0;
        for (int i = 0; i < 9; i++) {
            if (cellA->notes[i] != 0) {
                notes[point] = cellA->notes[i];
                point++;
            }
        }
        doublesNoteClean(cellA, cellB, notes[0], notes[1]);
    }
};


bool sudoku::pointingInBox(int num, int box) {
    //Search box for total number of num, if this is more than 2 then it returns false, else it will continue the function
    int total = 0;
    for (int i = 0; i < 9; i++) {
        cell* currentCell = accessBox(box, i);
        if (currentCell->notes[num - 1] == num && currentCell->num == 0) {
            total++;
        }
    }
    if (total != 2) {
        return false;
    }

    //Find locations of our 2 numbers
    cell* firstCell;
    cell* secondCell;
    bool second = false;
    for (int i = 0; i < 9; i++) {
        cell* currentCell = accessBox(box, i);
        if (currentCell->notes[num - 1] == num && second) {
            secondCell = currentCell;
        }
        if (currentCell->notes[num - 1] == num && !second) {
            firstCell = currentCell;
            second = true;
        }
    }

    //Check if the two cells are valid pointing pairs then cleans that row/column
    if (firstCell->row == secondCell->row || firstCell->col == secondCell->col) {
        doublesNoteClean(firstCell, secondCell, num, 0);
        //cout << "Pointing Pair Found" << num << " in " << box + 1 << endl;
        return true;
    }
    else {
        return false;
    }
}


//Solving Methods

void sudoku::noteScan() {
    for (int i = 0; i < filledEntries.size(); i++) {
        cell* currentCell = filledEntries.at(i);
        
        //scan Column
        int col = currentCell->col;
        for (int u = 0; u < 9; u++) {
            int looker = col + (9 * u);
            if (grid[looker].pos != currentCell->pos) {
                grid[looker].notes[currentCell->num - 1] = 0;
            }
        }
        
        //scan Row
        int row = currentCell->row;
        for (int u = 0; u < 9; u++) {
            int looker = (row * 9) + u;
            if (grid[looker].pos != currentCell->pos) {
                grid[looker].notes[currentCell->num - 1] = 0;
            }
        }
        
        //scan Box
        int box = getBoxHead(currentCell->box);
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                int looker = box + (x * 9) + y;
                if (grid[looker].pos != currentCell->pos) {
                    grid[looker].notes[currentCell->num - 1] = 0;
                }
            }
        }
        
    }
};


void sudoku::nakedSingles() {
    //Look for hot singles in your area
    for (int i = 0; i < 81; i++) {
        if (grid[i].num == 0) {
            int singles = 0;
            int sum = 0;
            for (int u = 0; u < 9; u++) {
                if (grid[i].notes[u] != 0) {        //finds the sum of all notes.
                    singles++;                      //Keeps track of # of notes and if that number is 
                    sum += grid[i].notes[u];        //one (there is only 1 note) then it fills in the value.
                }
            }
            if (singles == 1) {
                //cout << valid(sum, i);
                if (grid[i].num == 0) {
                    place(sum, i);
                    methodCycle();
                }
            }
        }
    }
};


void sudoku::hiddenSingles() {
    //Columns
    for (int x = 0; x < 9; x++) {
        array<int, 9> noteSum = columnSum(x);

        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                cell* currentCell = accessCol(x, y);
                if (contains(currentCell, i)) {
                    if (currentCell->num == 0) {
                        place(noteSum[i], currentCell->pos);
                        methodCycle();
                    }
                }
            }
        }
    }
    
    //Rows
    for (int x = 0; x < 9; x++) {
        array<int, 9> noteSum = rowSum(x);

        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                cell* currentCell = accessRow(x, y);
                if (contains(currentCell, i)) {
                    if (currentCell->num == 0) {
                        place(noteSum[i], currentCell->pos);
                        methodCycle();
                    }
                }
            }
        }
    }

    //Boxes
    for (int x = 0; x < 9; x++) {
        array<int, 9> noteSum = boxSum(x);

        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                cell* currentCell = accessBox(x, y);
                if (contains(currentCell, i)) {
                    if (currentCell->num == 0) {
                        place(noteSum[i], currentCell->pos);
                        methodCycle();
                    }
                }
            }
        }
    }
};


void sudoku::nakedDoubles() {
    //Columns
    for (int col = 0; col < 9; col++) {
        for (int cellTop = 0; cellTop < 8; cellTop++) {
            cell* top = accessCol(col, cellTop);
            for (int cellBottom = 1 + cellTop; cellBottom < 9; cellBottom++) {
                cell* bottom = accessCol(col, cellBottom);
                doublesEqual(top, bottom);
            }
        }
    }
    //Rows
    for (int row = 0; row < 9; row++) {
        for (int cellTop = 0; cellTop < 8; cellTop++) {
            cell* top = accessRow(row, cellTop);
            for (int cellBottom = 1 + cellTop; cellBottom < 9; cellBottom++) {
                cell* bottom = accessRow(row, cellBottom);
                doublesEqual(top, bottom);
            }
        }
    }
    //Boxes
    for (int box = 0; box < 9; box++) {
        for (int cellTop = 0; cellTop < 8; cellTop++) {
            cell* top = accessBox(box, cellTop);
            for (int cellBottom = 1 + cellTop; cellBottom < 9; cellBottom++) {
                cell* bottom = accessBox(box, cellBottom);
                doublesEqual(top, bottom);
            }
        }
    }
}


void sudoku::pointingPair() {
    for (int box = 0; box < 9; box++) {
        for (int num = 1; num <= 9; num++) {
            pointingInBox(num, box);
        }
    }
}


void sudoku::doublesNoteClean(cell* cellA, cell* cellB, int noteA, int noteB) {
    if (cellA->col == cellB->col) {
        int col = cellA->col;
        for (int i = 0; i < 9; i++) {
            cell* currentCell = accessCol(col, i);
            if (currentCell != cellA && currentCell != cellB) {         //removes the notes of the double cells from non double cells
                currentCell->notes[noteA - 1] = 0;
                if (noteB != 0) {
                    currentCell->notes[noteB - 1] = 0;
                }
            }
        }
    }
    if (cellA->row == cellB->row) {
        int row = cellA->row;
        for (int i = 0; i < 9; i++) {
            cell* currentCell = accessRow(row, i);
            if (currentCell != cellA && currentCell != cellB) {         //removes the notes of the double cells from non double cells
                currentCell->notes[noteA - 1] = 0;
                if (noteB != 0) {
                    currentCell->notes[noteB - 1] = 0;
                }
            }
        }
    }
    if (cellA->box == cellB->box) {
        int box = cellA->box;
        for (int i = 0; i < 9; i++) {
            cell* currentCell = accessBox(box, i);
            if (currentCell != cellA && currentCell != cellB) {         //removes the notes of the double cells from non double cells
                currentCell->notes[noteA - 1] = 0;
                if (noteB != 0) {
                    currentCell->notes[noteB - 1] = 0;
                }
            }
        }
    }                                                                   //ends with methodCycle OR NOT>!
}


void sudoku::rowColNoteClean(cell* cellA, cell* cellB, int note) {
    if (cellA->col == cellB->col) {
        int col = cellA->col;
        for (int i = 0; i < 9; i++) {
            cell* currentCell = accessCol(col, i);
            if (currentCell != cellA && currentCell != cellB) {         //removes the notes of the double cells from non double cells
                currentCell->notes[note - 1] = 0;
            }
        }
    }
    if (cellA->row == cellB->row) {
        int row = cellA->row;
        for (int i = 0; i < 9; i++) {
            cell* currentCell = accessRow(row, i);
            if (currentCell != cellA && currentCell != cellB) {         //removes the notes of the double cells from non double cells
                currentCell->notes[note - 1] = 0;
            }
        }
    }
}


void sudoku::xWing() {
    //Columns
    for (int colHead = 0; colHead < 8; colHead++) {
        array<int, 9> firstCol = columnSum(colHead);
        for (int colTail = colHead + 1; colTail < 9; colTail++) {
            array<int, 9> secondCol = columnSum(colTail);
            for (int num = 0; num < 9; num++) {
                if ((firstCol[num] == 2 * (num + 1)) && (secondCol[num] == 2 * (num + 1))) {
                    cell* topHead = 0;
                    cell* bottomHead = 0;
                    cell* topTail = 0;
                    cell* bottomTail = 0;
                    for (int i = 0; i < 9; i++) {
                        cell* tempCell = accessCol(colHead, i);
                        if (!topHead && contains(tempCell, num)) {
                            topHead = tempCell;
                        }
                        if (topHead && contains(tempCell, num)) {
                            topTail = tempCell;
                        }
                        tempCell = accessCol(colTail, i);
                        if (!bottomHead && contains(tempCell, num)) {
                            bottomHead = tempCell;
                        }
                        if (bottomHead && contains(tempCell, num)) {
                            bottomTail = tempCell;
                        }
                    }
                    if (topHead && bottomHead && topTail && bottomTail) {
                        if (topHead->row == bottomHead->row && topTail->row == bottomTail->row) {
                            /*
                            cout << "Eliminating X-Wing\nrows " << topHead->row << " and " << bottomTail->row << endl
                                 << "cols " << topHead->col << " and " << bottomHead->col << endl
                                 << "Note removed: " << num + 1 << endl;
                            */
                            rowColNoteClean(topHead, bottomHead, num + 1);
                            rowColNoteClean(topHead, topTail, num + 1);
                            rowColNoteClean(bottomHead, bottomTail, num + 1);
                            rowColNoteClean(topTail, bottomTail, num + 1);
                        }
                    }
                }
            }
        }
    }

    //Rows
    for (int rowHead = 0; rowHead < 8; rowHead++) {
        array<int, 9> firstRow = rowSum(rowHead);
        for (int rowTail = rowHead + 1; rowTail < 9; rowTail++) {
            array<int, 9> secondRow = rowSum(rowTail);
            for (int num = 0; num < 9; num++) {
                if ((firstRow[num] == 2 * (num + 1)) && (secondRow[num] == 2 * (num + 1))) {
                    
                    cell* topHead = 0;
                    cell* bottomHead = 0;
                    cell* topTail = 0;
                    cell* bottomTail = 0;
                    for (int i = 0; i < 9; i++) {
                        cell* tempCell = accessRow(rowHead, i);
                        if (!topHead && contains(tempCell, num)) {
                            topHead = tempCell;
                        }
                        if (topHead && contains(tempCell, num)) {
                            topTail = tempCell;
                        }
                        tempCell = accessRow(rowTail, i);
                        if (!bottomHead && contains(tempCell, num)) {
                            bottomHead = tempCell;
                        }
                        if (bottomHead && contains(tempCell, num)) {
                            bottomTail = tempCell;
                        }
                    }
                    if (topHead && bottomHead && topTail && bottomTail) {
                        if (topHead->col == bottomHead->col && topTail->col == bottomTail->col) {
                            /*
                            cout << "Eliminating X-Wing\nrows " << topHead->row << " and " << bottomTail->row << endl
                                 << "cols " << topHead->col << " and " << bottomHead->col << endl
                                 << "Note removed: " << num + 1 << endl;
                            */
                            rowColNoteClean(topHead, bottomHead, num + 1);
                            rowColNoteClean(topHead, topTail, num + 1);
                            rowColNoteClean(bottomHead, bottomTail, num + 1);
                            rowColNoteClean(topTail, bottomTail, num + 1);
                        }
                    }
                }
            }
        }
    }

};


void sudoku::methodCycle() {
    //notation modifiers
    noteScan();
    nakedDoubles();
    pointingPair();
    xWing();
    //displayGrid();
    //placers
    nakedSingles();
    hiddenSingles();


}


//Creating

void sudoku::resetNotes() {
    cell* current = 0;
    for (int i = 0; i < 81; i++) {
        current = accessGrid(i);
        if (current->num == 0) {
            for (int num = 0; num < 9; num++) {
                if (current->notes[num] == 0) {
                    current->notes[num] = num + 1;
                }
            }
        }
        else {

        }
    }
}


void sudoku::recursiveFillSudoku() {

    int num = 0;

    while(!complete()) {
        int random = rand();
        for (int i = 0; i < 9; i++) {
            num = randomValue(i, random);
            place(num, 0);
            recursiveFillSudoku(1);
            if (!complete()) {
                unplace();
            }
            else {
                return;
            }
        } 
    }
};


void sudoku::recursiveFillSudoku(int pos) {
    int num = 0;
    while(!complete() && pos < 81) {    //I believe this while loop can be replaced by an if statement only checking position
        int random = rand();
        for (int i = 0; i < 9; i++) {
            num = randomValue(i, random);
            if (valid(num, pos)) {
                place(num, pos);
                recursiveFillSudoku(pos + 1);
                //If the steps taken have not completed the sudoku, undo the move just made and try the rest of the values
                if (!complete()) {
                    unplace();
                }
                else {
                    return;
                }
            }
        }
        return;
    }
};


int sudoku::randomValue(int offset, int random) {
    return ((offset + random) % 9) + 1;      //gives a random number 1 - 9, at an offset of a random value.
}


//Debug and viewing

void sudoku::displayGrid() {
    string horizontal = "-------------------------\n";
    for (int i = 0; i < 81; i++) {
        if ((i / 9) % 3 == 0 && (i % 9) == 0) {
            cout << horizontal;
        }
        if (i % 3 == 0) {
            if (i % 9 == 0) {
                cout << "|";
            }
            else {
                cout << " |";
            }
        }

        cout << " " << grid[i].num;

        if (i % 9 == 8) {
            cout << " |\n";
        }

    }
    cout << horizontal;
};


void sudoku::peek(int pos) {
    for (int i = 0; i < 9; i++) {
        cout << grid[pos].notes[i] << " ";
    }
};


void sudoku::info(int pos) {
    cell cellIQ = grid[pos];
    cout << "Number: " << cellIQ.num << endl
         << "Row: " << cellIQ.row << endl
         << "Column: " << cellIQ.col << endl
         << "Notes: ";
    peek(pos);
    cout << endl << endl;
}




