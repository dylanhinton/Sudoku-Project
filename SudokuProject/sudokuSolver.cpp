#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>

using namespace std;


class sudoku {
    struct cell {
        int num, pos, col, row, box;
        int notes[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    };

public:
    cell grid[81];
    vector<cell*> filledEntries; //a list of pointers to completed cells

    sudoku() {

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
            }
            grid[i].pos = i;
            grid[i].col = getCol(i);
            grid[i].row = getRow(i);
            grid[i].box = getBox(i);
        }
        
    

    };
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

    bool complete();
    void place(int num, int pos);
    void noteScan();
    void nakedSingles();

    //debug
    void displayGrid();
    void peek(int pos);    //lists the notes at a cell
    void info(int pos);    //prints all relevant info for a cell (number, row, col, box, and notes)
    
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
        if (num = grid[(i * 9) + col].num) {
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



void sudoku::place(int num, int pos) {
    grid[pos].num = num;
    filledEntries.push_back(&grid[pos]);
};

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
                cout << valid(sum, i);
                place(sum, i);
                noteScan();
                nakedSingles();
            }
        }
    }
};



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
    cout << endl;
}


int main() {
    sudoku bob;
    bob.displayGrid();
    bob.noteScan();
    bob.nakedSingles();
    bob.displayGrid();
    bob.info(6);
    cout << bob.valid(6, 6);
    
    cout << "\n" << bob.getBoxHead(2);
    return 0;
};
