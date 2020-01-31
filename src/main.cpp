#include "grid.h"
int main () {
    auto gg = read_from_file("../../examples/sudoku.txt");
    std::cout << gg << '\n';

    return 0;
}