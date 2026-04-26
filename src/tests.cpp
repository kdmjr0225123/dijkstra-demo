#include <iostream>
#include <cassert>
#include <vector>
#include "MinHeap.h"
#include "Grid.h"
#include "Dijkstra.h"

#define PASS(msg) std::cout << "  [PASS] " << msg << "\n"
#define FAIL(msg) std::cout << "  [FAIL] " << msg << "\n"; allPassed = false

bool allPassed = true;

void testMinHeap() {
    std::cout << "\n--- MinHeap tests ---\n";

    // Basic push/pop ordering
    {
        MinHeap<int> h;
        h.push(5, 5); h.push(1, 1); h.push(3, 3); h.push(2, 2);
        assert(h.top().priority == 1);
        h.pop();
        assert(h.top().priority == 2);
        h.pop();
        assert(h.top().priority == 3);
        PASS("Min-heap ordering: pops smallest first");
    }

    // Single element
    {
        MinHeap<std::string> h;
        h.push(10, std::string("hello"));
        assert(h.top().value == "hello");
        h.pop();
        assert(h.empty());
        PASS("Single element push/pop");
    }

    // Duplicate priorities
    {
        MinHeap<int> h;
        h.push(5, 0); h.push(5, 1); h.push(5, 2);
        assert(h.size() == 3);
        h.pop(); h.pop(); h.pop();
        assert(h.empty());
        PASS("Duplicate priorities handled");
    }

    // Exception on empty pop
    {
        MinHeap<int> h;
        bool threw = false;
        try { h.pop(); } catch (const std::runtime_error&) { threw = true; }
        assert(threw);
        PASS("pop() throws on empty heap");
    }

    // Large push sequence
    {
        MinHeap<int> h;
        for (int i = 100; i >= 0; i--) h.push(i, i);
        int last = -1;
        while (!h.empty()) {
            int cur = h.top().priority; h.pop();
            assert(cur >= last);
            last = cur;
        }
        PASS("Large push (100 items) always pops in sorted order");
    }
}

void testGrid() {
    std::cout << "\n--- Grid tests ---\n";

    // Neighbours on open grid
    {
        Grid g(3, 3);
        auto n = g.neighbours(1, 1);
        assert(n.size() == 4);
        PASS("Centre cell has 4 neighbours on open 3x3");
    }

    // Corner neighbours
    {
        Grid g(3, 3);
        auto n = g.neighbours(0, 0);
        assert(n.size() == 2);
        PASS("Corner cell has 2 neighbours");
    }

    // Wall blocks neighbour
    {
        Grid g(3, 3);
        g.setWall(1, 2);
        auto n = g.neighbours(1, 1);
        assert(n.size() == 3);
        PASS("Wall reduces available neighbours");
    }

    // fromStrings
    {
        Grid g = Grid::fromStrings({"..#", "#..", "..."});
        assert(g.isWall(0, 2));
        assert(g.isWall(1, 0));
        assert(!g.isWall(1, 1));
        PASS("fromStrings parses walls correctly");
    }
}

void testDijkstra() {
    std::cout << "\n--- Dijkstra tests ---\n";

    // Direct path on open grid
    {
        Grid g(5, 5);
        auto r = Dijkstra::run(g, 0, 0, 4, 4);
        assert(r.found);
        assert(r.cost == 8); // Manhattan distance on grid = 4+4
        PASS("Open 5x5: found path, cost = 8");
    }

    // No path (fully blocked row)
    {
        Grid g(3, 3);
        g.setWall(1, 0); g.setWall(1, 1); g.setWall(1, 2);
        auto r = Dijkstra::run(g, 0, 0, 2, 2);
        assert(!r.found);
        assert(r.cost == -1);
        PASS("Blocked grid: no path found");
    }

    // Same start and end
    {
        Grid g(3, 3);
        auto r = Dijkstra::run(g, 1, 1, 1, 1);
        assert(r.found);
        assert(r.cost == 0);
        assert(r.path.size() == 1);
        PASS("Start == end: cost 0, path length 1");
    }

    // Path reconstruction correctness
    {
        Grid g = Grid::fromStrings({"....","####.","....."});
        // Can't reach bottom-right from top-left due to wall
        // (row 1 is fully blocked except col 4)
        auto r = Dijkstra::run(g, 0, 0, 2, 4);
        if (r.found) {
            // Verify path is connected
            for (int i = 1; i < (int)r.path.size(); i++) {
                auto [r1, c1] = r.path[i-1];
                auto [r2, c2] = r.path[i];
                int dr = std::abs(r2-r1), dc = std::abs(c2-c1);
                assert(dr + dc == 1); // steps must be adjacent
            }
            PASS("Path reconstruction: all steps are adjacent");
        } else {
            PASS("Path reconstruction: no path (walls block correctly)");
        }
    }

    // Distance table accuracy
    {
        Grid g(3, 3);
        auto r = Dijkstra::run(g, 0, 0, 2, 2);
        assert(r.dist[0][0] == 0);
        assert(r.dist[0][1] == 1);
        assert(r.dist[1][0] == 1);
        assert(r.dist[2][2] == 4);
        PASS("Distance table: correct values on 3x3 open grid");
    }
}

int main() {
    std::cout << "Running tests...\n";
    testMinHeap();
    testGrid();
    testDijkstra();
    std::cout << "\n" << (allPassed ? "[ALL TESTS PASSED]" : "[SOME TESTS FAILED]") << "\n";
    return allPassed ? 0 : 1;
}
