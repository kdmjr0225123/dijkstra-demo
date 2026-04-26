#include <iostream>
#include <iomanip>
#include <chrono>
#include "Grid.h"
#include "Dijkstra.h"

// ANSI colours (terminal only)
#define RESET  "\033[0m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define CYAN   "\033[36m"
#define RED    "\033[31m"
#define BOLD   "\033[1m"

void printDistTable(const std::vector<std::vector<int>>& dist,
                    const std::vector<std::pair<int,int>>& path,
                    int rows, int cols) {
    // Build path set for highlight
    std::vector<std::vector<bool>> onPath(rows, std::vector<bool>(cols, false));
    for (auto [r, c] : path) onPath[r][c] = true;

    std::cout << "\n  Distance table (INF = not reached, * = shortest path):\n\n";
    std::cout << "     ";
    for (int c = 0; c < cols; c++) std::cout << std::setw(5) << c;
    std::cout << "\n     ";
    for (int c = 0; c < cols; c++) std::cout << "-----";
    std::cout << '\n';

    for (int r = 0; r < rows; r++) {
        std::cout << std::setw(3) << r << " |";
        for (int c = 0; c < cols; c++) {
            int d = dist[r][c];
            if (onPath[r][c]) std::cout << GREEN << std::setw(4) << d << "*" << RESET;
            else if (d == Dijkstra::INF) std::cout << std::setw(5) << "INF";
            else std::cout << std::setw(5) << d;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

void runDemo(const std::string& label,
             const std::vector<std::string>& map,
             int sr, int sc, int er, int ec) {
    std::cout << BOLD << "\n=== " << label << " ===" << RESET << "\n\n";

    Grid g = Grid::fromStrings(map);
    g.print(sr, sc, er, ec);
    std::cout << "  (S=start, G=goal, #=wall)\n";

    auto t0 = std::chrono::high_resolution_clock::now();
    DijkstraResult r = Dijkstra::run(g, sr, sc, er, ec);
    auto t1 = std::chrono::high_resolution_clock::now();
    double us = std::chrono::duration<double, std::micro>(t1 - t0).count();

    std::cout << "\n  " << CYAN << "Result:" << RESET << "\n";
    if (r.found) {
        std::cout << "    Path found  : " << GREEN << "YES" << RESET << "\n";
        std::cout << "    Cost        : " << r.cost << " steps\n";
        std::cout << "    Path nodes  : " << r.path.size() << "\n";
        std::cout << "    Path        : ";
        for (auto [pr, pc] : r.path)
            std::cout << "(" << pr << "," << pc << ") ";
        std::cout << "\n";
    } else {
        std::cout << "    Path found  : " << RED << "NO PATH EXISTS" << RESET << "\n";
    }
    std::cout << "    Nodes visited  : " << r.nodesVisited << "\n";
    std::cout << "    Heap operations: " << r.heapOps << "\n";
    std::cout << "    Time           : " << std::fixed << std::setprecision(2) << us << " µs\n";

    printDistTable(r.dist, r.path, g.rows(), g.cols());
}

int main() {
    std::cout << BOLD << "Priority Queue-Based Pathfinding — Dijkstra Demo" << RESET << "\n";
    std::cout << "================================================\n";

    // --- Test 1: Simple open grid ---
    runDemo("Test 1: Open 5x5 grid",
    {
        ".....",
        ".....",
        ".....",
        ".....",
        "....."
    }, 0, 0, 4, 4);

    // --- Test 2: Maze with walls ---
    runDemo("Test 2: Maze (best-case route)",
    {
        "S...#",
        "###.#",
        "....#",
        ".###.",
        "....G"
    }, 0, 0, 4, 4);

    // --- Test 3: No path exists ---
    runDemo("Test 3: Blocked — no path",
    {
        "S.###",
        "..###",
        "#####",
        "###..",
        "###.G"
    }, 0, 0, 4, 4);

    // --- Test 4: Larger grid, worst-case snake ---
    runDemo("Test 4: 8x8 snake maze",
    {
        "S.......",
        "#######.",
        "........",
        ".#######",
        "........",
        "#######.",
        "........",
        ".......G"
    }, 0, 0, 7, 7);

    return 0;
}
