#pragma once

#include "SearchProblem.h"
#include "SearchAlgorithm.h"
#include <vector>
#include <string>

namespace ai_search {

enum class AppState { EDITING, VISUALIZING, FINISHED };
enum class ProblemType { MAZE, PUZZLE };

class Visualizer {
private:
    SearchProblem* problem_;
    SearchResult result_;
    
    int windowWidth_;
    int windowHeight_;
    int gridWidth_;
    int cellSize_;
    
    size_t historyIndex_;
    int framesCounter_;
    int framesPerStep_;

    std::vector<std::string> scoreboard_;
    AppState currentState_;
    ProblemType problemType_;
    
    void runAlgorithm(const std::string& algoName);
    
    void drawMaze();
    void drawPuzzle();

public:
    Visualizer(SearchProblem* problem, const SearchResult& result, int windowWidth = 1200, int windowHeight = 800);
    
    void setProblem(SearchProblem* problem);
    void setProblemType(ProblemType type);
    
    void clearResult();
    
    void render();
};

} // namespace ai_search
