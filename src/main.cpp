#include "grid.h"
int main () {
    auto gg = read_from_file("../../examples/sudoku4.txt");
    std::cout << gg << '\n';
    gg.solve();
    std::cout << gg << '\n';
    std::cout << (gg.check_validity()) << '\n';
    return 0;
}