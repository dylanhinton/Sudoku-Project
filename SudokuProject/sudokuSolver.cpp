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
                cout << valid(sum, i);
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
        int noteSum[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                noteSum[i] += grid[x + (y * 9)].notes[i];
            }
        }
        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                if (noteSum[i] == grid[x + (y * 9)].notes[i]) {
                    if (grid[x + (y * 9)].num == 0) {
                        place(noteSum[i], x + (y * 9));
                        methodCycle();
                    }
                }
            }
        }
    }
    //Rows
    for (int x = 0; x < 9; x++) {
        int noteSum[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                noteSum[i] += grid[(x * 9) + y].notes[i];
            }
        }
        for (int y = 0; y < 9; y++) {
            for (int i = 0; i < 9; i++) {
                if (noteSum[i] == grid[(x * 9) + y].notes[i]) {
                    if (grid[(x * 9) + y].num == 0) {
                        place(noteSum[i], (x * 9) + y);
                        methodCycle();
                    }
                }
            }
        }
    }
    //Boxs
    for (int b = 0; b < 9; b++) {
        int boxHead = getBoxHead(b);
        int noteSum[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                for (int i = 0; i < 9; i++) {
                    noteSum[i] += grid[boxHead + x + (9 * y)].notes[i];
                }
            }
        }
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                for (int i = 0; i < 9; i++) {
                    if (noteSum[i] == grid[boxHead + x + (9 * y)].notes[i]) {
                        if (grid[boxHead + x + (9 * y)].num == 0) {
                            place(noteSum[i], boxHead + x + (9 * y));
                            methodCycle();
                        }
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


void sudoku::methodCycle() {
    //notation modifiers
    noteScan();
    nakedDoubles();
    //placers
    nakedSingles();
    hiddenSingles();
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




int main() {
    sudoku bob;
    bob.displayGrid();
    bob.methodCycle();
    bob.displayGrid();
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            bob.info((x * 9) + y + 54);
        }
    }
    cout << bob.valid(6, 6);
    
    cout << "\n" << bob.getBoxHead(2);
    return 0;
};
