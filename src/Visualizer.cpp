#include "../include/Visualizer.h"
#include "../include/GridProblem.h"
#include "../include/EightPuzzleProblem.h"
#include "../include/BFS.h"
#include "../include/DFS.h"
#include "../include/IDDFS.h"
#include "../include/MDFBB.h"
#include "../include/AStar.h"
#include <raylib.h>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace ai_search {

Visualizer::Visualizer(SearchProblem* problem, const SearchResult& result, int windowWidth, int windowHeight)
    : problem_(problem), result_(result), windowWidth_(windowWidth), windowHeight_(windowHeight), gridWidth_(800), historyIndex_(0), framesCounter_(0), framesPerStep_(2), currentState_(AppState::EDITING), problemType_(ProblemType::MAZE) {
    
    // Initialize scoreboard if we have initial results
    if (result.success || result.nodesExpanded > 0) {
        std::string prefix = (problemType_ == ProblemType::PUZZLE) ? "Puzzle - " : "Maze - ";
        std::stringstream ss;
        ss << prefix << "A*: ";
        if (result.path.empty()) ss << "No Path, ";
        else ss << result.totalCost << " cost, ";
        ss << result.nodesExpanded << " nodes, " 
           << std::fixed << std::setprecision(1) << result.executionTimeMs << "ms";
        scoreboard_.push_back(ss.str());
    }
}

void Visualizer::setProblemType(ProblemType type) {
    problemType_ = type;
}

void Visualizer::setProblem(SearchProblem* problem) {
    problem_ = problem;
}

void Visualizer::clearResult() {
    result_ = SearchResult();
    historyIndex_ = 0;
    framesCounter_ = 0;
    currentState_ = AppState::EDITING;
}

void Visualizer::runAlgorithm(const std::string& algoName) {
    SearchResult newResult;
    
    if (algoName == "BFS") {
        BFS algo;
        newResult = algo.solve(*problem_);
    } else if (algoName == "DFS") {
        DFS algo;
        newResult = algo.solve(*problem_);
    } else if (algoName == "IDDFS") {
        IDDFS algo;
        newResult = algo.solve(*problem_);
    } else if (algoName == "MDFBB") {
        MDFBB algo;
        newResult = algo.solve(*problem_);
    } else if (algoName == "AStar") {
        AStar algo;
        newResult = algo.solve(*problem_);
    }

    result_ = newResult;
    historyIndex_ = 0;
    framesCounter_ = 0;
    currentState_ = AppState::VISUALIZING;

    std::string prefix = (problemType_ == ProblemType::PUZZLE) ? "Puzzle - " : "Maze - ";
    std::string displayName = (algoName == "AStar") ? "A*" : algoName;
    std::stringstream ss;
    ss << prefix << displayName << ": ";
    if (newResult.path.empty()) ss << "No Path, ";
    else ss << newResult.totalCost << " cost, ";
    ss << newResult.nodesExpanded << " nodes, " 
       << std::fixed << std::setprecision(1) << newResult.executionTimeMs << "ms";
    scoreboard_.push_back(ss.str());
}

void Visualizer::drawMaze() {
    auto gridProb = dynamic_cast<GridProblem*>(problem_);
    if (!gridProb) return;

    const auto& map = gridProb->getMap();
    int rows = map.size();
    int cols = map.empty() ? 0 : map[0].size();
    
    int cellWidth = gridWidth_ / (cols > 0 ? cols : 1);
    int cellHeight = windowHeight_ / (rows > 0 ? rows : 1);
    cellSize_ = std::min(cellWidth, cellHeight);
    
    int offsetX = (gridWidth_ - (cols * cellSize_)) / 2;
    int offsetY = (windowHeight_ - (rows * cellSize_)) / 2;

    // Interactive Mouse Inputs for Editing
    if (currentState_ == AppState::EDITING) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();

            if (mouseX >= offsetX && mouseX < offsetX + cols * cellSize_ &&
                mouseY >= offsetY && mouseY < offsetY + rows * cellSize_) {
                
                int gridX = (mouseX - offsetX) / cellSize_;
                int gridY = (mouseY - offsetY) / cellSize_;
                
                gridProb->toggleWall(gridX, gridY);
            }
        }
    }

    // 1. Draw the Base Grid
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            char cell = map[y][x];
            Color color = LIGHTGRAY; // Empty space '.'
            
            if (cell == '#') color = DARKGRAY;
            else if (cell == 'S') color = GREEN;
            else if (cell == 'G') color = RED;

            DrawRectangle(offsetX + x * cellSize_, offsetY + y * cellSize_, cellSize_, cellSize_, color);
            DrawRectangleLines(offsetX + x * cellSize_, offsetY + y * cellSize_, cellSize_, cellSize_, GRAY);
        }
    }

    // 2. Draw Explored Nodes (Animation)
    for (size_t i = 0; i < historyIndex_; ++i) {
        auto gridState = std::dynamic_pointer_cast<GridState>(result_.explorationHistory[i]);
        if (gridState) {
            char cell = map[gridState->y][gridState->x];
            if (cell != 'S' && cell != 'G') {
                DrawRectangle(offsetX + gridState->x * cellSize_, offsetY + gridState->y * cellSize_, cellSize_, cellSize_, SKYBLUE);
                DrawRectangleLines(offsetX + gridState->x * cellSize_, offsetY + gridState->y * cellSize_, cellSize_, cellSize_, GRAY);
            }
        }
    }

    // 3. Draw the Final Path once the exploration animation completes
    if (currentState_ == AppState::FINISHED || historyIndex_ >= result_.explorationHistory.size()) {
        for (const auto& state : result_.path) {
            auto gridState = std::dynamic_pointer_cast<GridState>(state);
            if (gridState) {
                char cell = map[gridState->y][gridState->x];
                if (cell != 'S' && cell != 'G') {
                    DrawRectangle(offsetX + gridState->x * cellSize_, offsetY + gridState->y * cellSize_, cellSize_, cellSize_, YELLOW);
                    DrawRectangleLines(offsetX + gridState->x * cellSize_, offsetY + gridState->y * cellSize_, cellSize_, cellSize_, ORANGE);
                }
            }
        }
    }
}

void Visualizer::drawPuzzle() {
    if (currentState_ == AppState::EDITING) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int mouseX = GetMouseX();
            int mouseY = GetMouseY();
            if (mouseX >= 0 && mouseX < gridWidth_ && mouseY >= 0 && mouseY < windowHeight_) {
                int clickedCol = mouseX / (800 / 3);
                int clickedRow = mouseY / (800 / 3);
                auto puzzleProb = dynamic_cast<EightPuzzleProblem*>(problem_);
                if (puzzleProb) puzzleProb->manuallySwapTile(clickedRow, clickedCol);
            }
        }
    }

    std::shared_ptr<State> currentStateToDraw = nullptr;

    if (result_.explorationHistory.empty()) {
        currentStateToDraw = problem_->getStartState();
    } else {
        size_t idx = (historyIndex_ < result_.explorationHistory.size()) ? historyIndex_ : (result_.explorationHistory.size() - 1);
        currentStateToDraw = result_.explorationHistory[idx];
    }

    auto puzzleState = std::dynamic_pointer_cast<EightPuzzleState>(currentStateToDraw);
    if (!puzzleState) return;

    float cellSize = 800.0f / 3.0f;

    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;
        int val = puzzleState->board[i];

        float x = col * cellSize;
        float y = row * cellSize;

        if (val == 0) {
            DrawRectangle(x, y, cellSize, cellSize, DARKGRAY);
        } else {
            DrawRectangle(x, y, cellSize, cellSize, BLUE);
            DrawRectangleLines(x, y, cellSize, cellSize, BLACK);
            
            std::string text = std::to_string(val);
            int fontSize = 60;
            int textWidth = MeasureText(text.c_str(), fontSize);
            DrawText(text.c_str(), x + (cellSize - textWidth) / 2.0f, y + (cellSize - fontSize) / 2.0f, fontSize, WHITE);
        }
    }
}

void Visualizer::render() {
    // Interactive Keyboard Inputs
    if (currentState_ == AppState::EDITING || currentState_ == AppState::FINISHED) {
        if (IsKeyPressed(KEY_C)) {
            clearResult();
        }
        else if (IsKeyPressed(KEY_B)) runAlgorithm("BFS");
        else if (IsKeyPressed(KEY_D)) runAlgorithm("DFS");
        else if (IsKeyPressed(KEY_I)) runAlgorithm("IDDFS");
        else if (IsKeyPressed(KEY_M)) runAlgorithm("MDFBB");
        else if (IsKeyPressed(KEY_A)) runAlgorithm("AStar");
        else if (IsKeyPressed(KEY_R) && problemType_ == ProblemType::PUZZLE) {
            auto puzzleProb = dynamic_cast<EightPuzzleProblem*>(problem_);
            if (puzzleProb) puzzleProb->randomizeBoard();
        }
    }

    if (currentState_ == AppState::VISUALIZING) {
        framesCounter_++;
        int threshold = (problemType_ == ProblemType::PUZZLE) ? 30 : 2;
        if (framesCounter_ >= threshold) {
            framesCounter_ = 0;
            if (historyIndex_ < result_.explorationHistory.size()) {
                historyIndex_++;
            } else {
                currentState_ = AppState::FINISHED;
            }
        }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (problemType_ == ProblemType::MAZE) {
        drawMaze();
    } else if (problemType_ == ProblemType::PUZZLE) {
        drawPuzzle();
    }

    // Draw UI Panel (Untouched and functional for both modes)
    DrawRectangle(gridWidth_, 0, windowWidth_ - gridWidth_, windowHeight_, DARKGRAY);
    
    int padding = 20;
    int currentY = padding;
    
    DrawText("AI Search Sandbox", gridWidth_ + padding, currentY, 30, WHITE);
    currentY += 50;
    
    std::string modeText = "State: ";
    if (currentState_ == AppState::EDITING) modeText += "EDITING";
    else if (currentState_ == AppState::VISUALIZING) modeText += "SEARCHING";
    else if (currentState_ == AppState::FINISHED) modeText += "FINISHED";
    
    DrawText(modeText.c_str(), gridWidth_ + padding, currentY, 20, YELLOW);
    currentY += 40;
    
    DrawText("Controls:", gridWidth_ + padding, currentY, 20, LIGHTGRAY);
    currentY += 30;
    DrawText("[B] BFS     [D] DFS", gridWidth_ + padding, currentY, 18, RAYWHITE);
    currentY += 25;
    DrawText("[I] IDDFS   [M] MDFBB", gridWidth_ + padding, currentY, 18, RAYWHITE);
    currentY += 25;
    DrawText("[A] A*      [C] Clear Grid", gridWidth_ + padding, currentY, 18, RAYWHITE);
    currentY += 25;
    DrawText("[P] Puzzle  [G] Maze", gridWidth_ + padding, currentY, 18, SKYBLUE);
    currentY += 25;
    DrawText("[R] Randomize (Puzzle)", gridWidth_ + padding, currentY, 18, ORANGE);
    currentY += 50;

    DrawText("Scoreboard:", gridWidth_ + padding, currentY, 24, WHITE);
    currentY += 40;

    for (auto it = scoreboard_.rbegin(); it != scoreboard_.rend(); ++it) {
        DrawText(it->c_str(), gridWidth_ + padding, currentY, 16, GREEN);
        currentY += 25;
        if (currentY > windowHeight_ - padding) break; // Avoid drawing off-screen
    }

    EndDrawing();
}

} // namespace ai_search
