A hand-written min-heap and Dijkstra's shortest-path algorithm implemented from scratch, with an interactive browser-based visualizer.

## Live Demo
https://kdmjr0225123.github.io/dijkstra-demo/demo/index.html

Project Structure
- include/ — MinHeap, Grid, and Dijkstra header files
- src/ — implementation and test files
- demo/ — browser visualizer (HTML/JS)

How to Build
g++ -std=c++17 -O2 -Iinclude src/Grid.cpp src/Dijkstra.cpp src/main.cpp -o dijkstra_demo
./dijkstra_demo

Run Tests
g++ -std=c++17 -O2 -Iinclude src/Grid.cpp src/Dijkstra.cpp src/tests.cpp -o dijkstra_tests
./dijkstra_tests

What It Demonstrates
- How a priority queue drives shortest-path selection
- Node relaxation step by step
- Full distance table visualization
- Best, average, and worst case performance
