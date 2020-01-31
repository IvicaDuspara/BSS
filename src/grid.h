#ifndef BSS_GRID_H
#define BSS_GRID_H
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <iterator>

class ClassicGrid{
public:
    ClassicGrid (){};
    explicit ClassicGrid(std::array<std::array<int,9>,9> _grid) : grid{_grid} {
        initial_set_fill();
    };

    std::array<std::array<int,9>,9>& get_grid() { return grid;}

    void initial_set_fill();
    friend std::ostream& operator<< (std::ostream&, const ClassicGrid& _grid);
private:

    std::array<std::array<int,9>,9> grid;
    std::array<std::array<std::set<int>,9>,9> available_numbers;
};


void ClassicGrid::initial_set_fill() {

}

std::ostream& operator<< (std::ostream&out, const ClassicGrid& _grid) {
    out << "- - - - + - - - + - - - -\n";
    for(int i = 0; i < 9; i++) {
        std::cout << "| ";
        for(int j = 0; j < 9; j++) {
            if(_grid.grid[i][j]) {
                out << _grid.grid[i][j] << " ";
            }
            else {
                out << "  ";
            }
            if(j == 2 || j == 5 || j == 8) {
                out << "| ";
            }
        }
        out << '\n';
        if(i == 2 || i == 5 || i == 8) {
            out << "- - - - + - - - + - - - -\n";
        }
    }
    return out;
}

template <typename S>
ClassicGrid read_from_file(S&& file_name) {
    ClassicGrid g;
    std::ifstream in_file(std::forward<S>(file_name));
    if(!in_file) {
        std::cerr << "Could not open a file at path: " << file_name << std::endl;
        exit(-1);
    }
    unsigned long row = 0;
    unsigned long column = 0;
    for(std::string line; getline(in_file,line);) {
        std::stringstream iss(line);
        std::istream_iterator<std::string> iStreamIterator(iss);
        std::istream_iterator<std::string> eof;
        while(iStreamIterator != eof) {
            auto read = *iStreamIterator;
            if(read == "-") {
                g.get_grid()[row][column] = 0;
            }
            else {
                g.get_grid()[row][column] = std::stoi(read);
            }
            iStreamIterator++;
            column++;
        }
        column = 0;
        row++;
    }
    g.initial_set_fill();
    return g;
}
#endif //BSS_GRID_H
