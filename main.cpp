#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>

#include "include/GridProblem.h"
#include "include/EightPuzzleProblem.h"
#include "include/AStar.h"
#include "include/Visualizer.h"

using namespace ai_search;

int main() {
    // 1. Setup Maze Problem
    std::vector<std::string> mazeMap = {
        "S.......",
        "####.###",
        ".....#..",
        ".#.#.##.",
        ".#.#....",
        "##.####.",
        "........",
        ".......G"
    };
    GridProblem gridProblem(mazeMap);
    
    // 2. Setup Eight Puzzle Problem
    std::vector<int> puzzleBoard = {1, 2, 5, 3, 4, 0, 6, 7, 8};
    EightPuzzleProblem puzzleProblem(puzzleBoard);

    // We start with an empty result since the app starts in EDITING mode
    SearchResult emptyResult;
    
    // Initialize Raylib with 1200x800 for the Sandbox
    InitWindow(1200, 800, "AI Search Sandbox");
    SetTargetFPS(60);

    // Instantiate Visualizer pointing to the grid initially
    Visualizer visualizer(&gridProblem, emptyResult, 1200, 800);
    visualizer.setProblemType(ProblemType::MAZE);

    // Raylib Render Loop
    while (!WindowShouldClose()) {
        // App-level toggles for swapping problems
        if (IsKeyPressed(KEY_P)) {
            visualizer.setProblem(&puzzleProblem);
            visualizer.setProblemType(ProblemType::PUZZLE);
            visualizer.clearResult();
        } else if (IsKeyPressed(KEY_G)) {
            visualizer.setProblem(&gridProblem);
            visualizer.setProblemType(ProblemType::MAZE);
            visualizer.clearResult();
        }

        visualizer.render();
    }

    // Cleanup
    CloseWindow();

    return 0;
}
