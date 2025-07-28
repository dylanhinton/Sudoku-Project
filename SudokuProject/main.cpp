#include "sudokuSolver.hpp"
#include <iostream>
using namespace std;

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
