#ifndef BSS_GRID_H
#define BSS_GRID_H
#include <string>
#include <array>
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <iterator>
constexpr int cg_height = 9;
constexpr int cg_width = 9;
constexpr int valid_sum = 45;
class ClassicGrid{
public:
    ClassicGrid(){};
    explicit ClassicGrid(std::array<std::array<int,9>,9> _grid) : grid{_grid} {
        initial_set_fill();
    };

    std::array<std::array<int,9>,9>& get_grid() { return grid;}

    void initial_set_fill();
    void print_allowed_numbers();
    void solve();
    bool check_validity();
    friend std::ostream& operator<< (std::ostream&, const ClassicGrid& _grid);
private:
    void remove_from_column(int column, int removed);
    void remove_from_row(int row, int removed);
    void remove_from_house(int row, int column, int removed);
    bool first_stage_removal();
    bool second_stage_removal();
    std::array<std::array<int,cg_width>,cg_height> grid;
    std::array<std::array<std::set<int>,cg_width>,cg_height> allowed_numbers;
};
/*========================================================= Public methods: =========================================*/
void ClassicGrid::print_allowed_numbers() {
    for(int row = 0; row < cg_height; row++) {
        std::cout << "Row " << (row+1) <<'\n';
        for(int column = 0; column < cg_width; column++) {
            std::cout << "\t\tcolumn " << (column+1) << " can contain: ";
            for(const auto& element : allowed_numbers[row][column]) {
                std::cout << element << ' ';
            }
            std::cout << "\n";
        }
    }
}

void ClassicGrid::initial_set_fill() {
    for(int row = 0; row < cg_height; row++) {
        for(int column = 0; column < cg_width; column++) {
            allowed_numbers[row][column] = std::set<int>{1,2,3,4,5,6,7,8,9};
        }
    }
    for(int row = 0; row < cg_height; row++){
        for(int column = 0; column < cg_width; column++) {
            int removed = grid[row][column];
            if(!removed) {
                continue;
            }
            else {
                remove_from_row(row,removed);
                remove_from_column(column,removed);
                remove_from_house(row,column,removed);
            }
        }
    }
    for(int row = 0; row < cg_height; row++) {
        for(int column = 0; column < cg_width; column++) {
            if(grid[row][column]) {
                allowed_numbers[row][column] = std::set<int>{};
            }
        }
    }
}

void ClassicGrid::solve() {
    first_stage_removal();
}

bool ClassicGrid::check_validity() {
    for(int i = 0; i < cg_height; i++) {
        int sum = 0;
        for(int j = 0; j < cg_width; j++) {
            sum += grid[i][j];
        }
        if(sum != valid_sum) {
            return false;
        }
    }
    for(int i = 0; i < cg_height; i++) {
        int sum = 0;
        for(int j = 0; j < cg_width; j++) {
            sum += grid[j][i];
        }
        if(sum != valid_sum) {
            return false;
        }
    }
    std::set<int> house_rows{0,3,6};
    std::set<int> house_columns{0,3,6};
    for(const auto& hr : house_rows) {
        for(const auto& hc : house_columns) {
            int sum = 0;
            for(int i = hr; i < (hr+3); i++) {
                for(int j = hc; j < (hc+3); j++) {
                    sum += grid[i][j];
                }
            }
            if(sum != valid_sum) {
                return false;
            }
        }
    }
    return true;
}

/*========================================================= Private methods:  ========================================*/
void ClassicGrid::remove_from_row(int row, int removed) {
    for(int i = 0; i < cg_width; i++) {
        allowed_numbers[row][i].erase(removed);
    }
}

void ClassicGrid::remove_from_column(int column, int removed) {
    for(int i = 0; i < cg_height; i++) {
        allowed_numbers[i][column].erase(removed);
    }
}

void ClassicGrid::remove_from_house(int row, int column, int removed) {
    int start_row = 0, start_column = 0;
    if(row > 2 && row < 6) {
        start_row = 3;
    }
    else if(row >= 6) {
        start_row = 6;
    }
    if(column > 2 && column < 6) {
        start_column = 3;
    }
    else if(column >= 6) {
        start_column = 6;
    }
    for(int sr = start_row; sr < (start_row + 3); sr++) {
        for(int sc = start_column; sc < (start_column + 3); sc++) {
            allowed_numbers[sr][sc].erase(removed);
        }
    }
}

bool ClassicGrid::first_stage_removal() {
    bool flag = false;
    for(int i = 0; i < cg_height; i++) {
        for(int j = 0; j < cg_width; j++) {
            if(allowed_numbers[i][j].size() == 1) {
                flag = true;
                auto removed = *allowed_numbers[i][j].begin();
                remove_from_row(i,removed);
                remove_from_column(j,removed);
                remove_from_house(i,j,removed);
                grid[i][j] = removed;
                allowed_numbers[i][j] = std::set<int>{};
                i = 0;
                j = -1;
            }
        }
    }
    return flag;
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
