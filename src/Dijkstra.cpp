#include "Dijkstra.h"
#include <algorithm>

DijkstraResult Dijkstra::run(const Grid& grid,
                              int startR, int startC,
                              int endR,   int endC) {
    int R = grid.rows(), C = grid.cols();

    // Distance table: INF = not yet reached
    std::vector<std::vector<int>> dist(R, std::vector<int>(C, INF));

    // Previous-node table for path reconstruction
    std::vector<std::vector<std::pair<int,int>>> prev(
        R, std::vector<std::pair<int,int>>(C, {-1, -1}));

    // Visited array: once a node is popped its distance is final
    std::vector<std::vector<bool>> visited(R, std::vector<bool>(C, false));

    int nodesVisited = 0, heapOps = 0;

    // Min-heap stores (distance, encoded cell index)
    // Encoding: r * C + c  keeps it a single int
    MinHeap<int> heap;
    dist[startR][startC] = 0;
    heap.push(0, startR * C + startC);
    heapOps++;

    bool found = false;

    while (!heap.empty()) {
        auto [d, encoded] = heap.top();
        heap.pop();
        heapOps++;

        int r = encoded / C;
        int c = encoded % C;

        if (visited[r][c]) continue;
        visited[r][c] = true;
        nodesVisited++;

        // Goal check
        if (r == endR && c == endC) { found = true; break; }

        // Relax neighbours
        for (auto [nr, nc] : grid.neighbours(r, c)) {
            if (visited[nr][nc]) continue;
            int nd = d + 1;  // uniform edge weight = 1
            if (nd < dist[nr][nc]) {
                dist[nr][nc]  = nd;
                prev[nr][nc]  = {r, c};
                heap.push(nd, nr * C + nc);
                heapOps++;
            }
        }
    }

    DijkstraResult result;
    result.found        = found;
    result.dist         = dist;
    result.nodesVisited = nodesVisited;
    result.heapOps      = heapOps;

    if (found) {
        result.path = tracePath(prev, startR, startC, endR, endC);
        result.cost = dist[endR][endC];
    } else {
        result.cost = -1;
    }

    return result;
}

std::vector<std::pair<int,int>>
Dijkstra::tracePath(const std::vector<std::vector<std::pair<int,int>>>& prev,
                    int startR, int startC, int endR, int endC) {
    std::vector<std::pair<int,int>> path;
    std::pair<int,int> cur = {endR, endC};
    while (cur != std::make_pair(-1, -1)) {
        path.push_back(cur);
        auto [r, c] = cur;
        cur = prev[r][c];
        // Guard against infinite loop (shouldn't happen if found=true)
        if (cur == std::make_pair(r, c)) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
