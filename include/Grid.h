#pragma once
#include <vector>
#include <string>
#include <utility>

/**
 * Grid.h
 * Represents an R x C grid graph where each cell is a node.
 * Edges connect adjacent (up/down/left/right) non-wall cells with weight 1.
 *
 * Cell states:
 *   0 = open
 *   1 = wall (blocked)
 */
class Grid {
public:
    static constexpr int WALL = 1;
    static constexpr int OPEN = 0;

    Grid(int rows, int cols);

    int  rows() const { return rows_; }
    int  cols() const { return cols_; }

    bool isWall(int r, int c)  const;
    bool inBounds(int r, int c) const;
    void setWall(int r, int c, bool wall = true);

    // Returns open neighbours (r, c) of cell (r0, c0)
    std::vector<std::pair<int,int>> neighbours(int r, int c) const;

    // Load grid from a vector of strings ('.' = open, '#' = wall)
    static Grid fromStrings(const std::vector<std::string>& rows);

    // Pretty-print to stdout
    void print(int startR = -1, int startC = -1,
               int endR   = -1, int endC   = -1) const;

private:
    int rows_, cols_;
    std::vector<std::vector<int>> cells_;
};
