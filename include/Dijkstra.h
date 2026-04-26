#pragma once
#include "Grid.h"
#include "MinHeap.h"
#include <vector>
#include <utility>

/**
 * Dijkstra.h
 * Single-source shortest path on a Grid using our hand-written MinHeap.
 *
 * Algorithm overview:
 *   1. Initialise dist[start] = 0, all others = INF
 *   2. Push start onto the min-heap
 *   3. Pop the node u with the smallest dist
 *   4. For each neighbour v of u:
 *        if dist[u] + 1 < dist[v]:
 *            dist[v] = dist[u] + 1
 *            prev[v] = u          (for path reconstruction)
 *            push v onto heap
 *   5. Repeat from 3 until heap is empty or goal is popped
 *
 * Time  complexity: O((V + E) log V)   V = rows*cols, E = edges
 * Space complexity: O(V)
 */

struct DijkstraResult {
    bool   found;                          // Was a path found?
    int    cost;                           // Total path cost (-1 if not found)
    std::vector<std::pair<int,int>> path;  // Cells from start to end (inclusive)
    std::vector<std::vector<int>> dist;    // Full distance array (INF = not reached)
    int    nodesVisited;                   // How many nodes were popped from heap
    int    heapOps;                        // Total push + pop operations
};

class Dijkstra {
public:
    static constexpr int INF = 1e9;

    /**
     * Run Dijkstra from (startR, startC) to (endR, endC) on the given grid.
     * Returns a DijkstraResult with the shortest path and full diagnostics.
     */
    static DijkstraResult run(const Grid& grid,
                              int startR, int startC,
                              int endR,   int endC);

private:
    // Reconstruct path from prev table
    static std::vector<std::pair<int,int>>
    tracePath(const std::vector<std::vector<std::pair<int,int>>>& prev,
              int startR, int startC, int endR, int endC);
};
