#include "Grid.h"
#include <iostream>
#include <stdexcept>

Grid::Grid(int rows, int cols)
    : rows_(rows), cols_(cols),
      cells_(rows, std::vector<int>(cols, OPEN)) {}

bool Grid::inBounds(int r, int c) const {
    return r >= 0 && r < rows_ && c >= 0 && c < cols_;
}

bool Grid::isWall(int r, int c) const {
    if (!inBounds(r, c)) throw std::out_of_range("Grid::isWall out of range");
    return cells_[r][c] == WALL;
}

void Grid::setWall(int r, int c, bool wall) {
    if (!inBounds(r, c)) throw std::out_of_range("Grid::setWall out of range");
    cells_[r][c] = wall ? WALL : OPEN;
}

std::vector<std::pair<int,int>> Grid::neighbours(int r, int c) const {
    static const int DR[] = {-1,  1,  0,  0};
    static const int DC[] = { 0,  0, -1,  1};
    std::vector<std::pair<int,int>> result;
    for (int d = 0; d < 4; d++) {
        int nr = r + DR[d], nc = c + DC[d];
        if (inBounds(nr, nc) && cells_[nr][nc] == OPEN)
            result.push_back({nr, nc});
    }
    return result;
}

Grid Grid::fromStrings(const std::vector<std::string>& rows) {
    if (rows.empty()) throw std::invalid_argument("Grid::fromStrings: empty input");
    Grid g(rows.size(), rows[0].size());
    for (int r = 0; r < (int)rows.size(); r++)
        for (int c = 0; c < (int)rows[r].size(); c++)
            if (rows[r][c] == '#') g.setWall(r, c);
    return g;
}

void Grid::print(int startR, int startC, int endR, int endC) const {
    for (int r = 0; r < rows_; r++) {
        for (int c = 0; c < cols_; c++) {
            if      (r == startR && c == startC) std::cout << 'S';
            else if (r == endR   && c == endC)   std::cout << 'G';
            else if (cells_[r][c] == WALL)        std::cout << '#';
            else                                  std::cout << '.';
        }
        std::cout << '\n';
    }
}
