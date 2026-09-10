#include "../include/GridProblem.h"
#include <iostream>
#include <stdexcept>
#include <cmath>

namespace ai_search {

// ---------------- GridState Implementation ----------------

GridState::GridState(int x, int y) : x(x), y(y) {}

std::string GridState::getHash() const {
    return std::to_string(x) + "," + std::to_string(y);
}

void GridState::print() const {
    std::cout << "GridState(x=" << x << ", y=" << y << ")\n";
}


// ---------------- GridProblem Implementation ----------------

GridProblem::GridProblem(const std::vector<std::string>& map) : map_(map) {
    // Pre-calculate goal coordinates to be used by the heuristic
    for (int y = 0; y < map_.size(); ++y) {
        for (int x = 0; x < map_[y].size(); ++x) {
            if (map_[y][x] == 'G') {
                goalX_ = x;
                goalY_ = y;
            }
        }
    }
}

std::shared_ptr<State> GridProblem::getStartState() const {
    // Scan the map to find the start state 'S'
    for (int y = 0; y < map_.size(); ++y) {
        for (int x = 0; x < map_[y].size(); ++x) {
            if (map_[y][x] == 'S') {
                return std::make_shared<GridState>(x, y);
            }
        }
    }
    throw std::runtime_error("Start state 'S' not found in the provided map.");
}

bool GridProblem::isGoalState(const std::shared_ptr<State>& state) const {
    auto gridState = std::dynamic_pointer_cast<GridState>(state);
    if (!gridState) return false;

    int x = gridState->x;
    int y = gridState->y;

    // Check bounds before accessing the map
    if (y >= 0 && y < map_.size() && x >= 0 && x < map_[y].size()) {
        // Scan the map position to see if it's the goal 'G'
        return map_[y][x] == 'G';
    }
    
    return false;
}

std::vector<Action> GridProblem::getSuccessors(const std::shared_ptr<State>& state) const {
    std::vector<Action> successors;
    auto gridState = std::dynamic_pointer_cast<GridState>(state);
    
    if (!gridState) return successors;

    int x = gridState->x;
    int y = gridState->y;

    // Directions: UP, DOWN, LEFT, RIGHT
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Ensure the new coordinates are within bounds
        if (ny >= 0 && ny < map_.size() && nx >= 0 && nx < map_[ny].size()) {
            // Ensure we are not hitting a wall
            if (map_[ny][nx] != '#') {
                Action action;
                action.nextState = std::make_shared<GridState>(nx, ny);
                action.cost = 1.0;
                successors.push_back(action);
            }
        }
    }

    return successors;
}

double GridProblem::getHeuristic(const std::shared_ptr<State>& state) const {
    auto gridState = std::dynamic_pointer_cast<GridState>(state);
    if (!gridState || goalX_ == -1 || goalY_ == -1) {
        return 0.0;
    }

    // Manhattan Distance calculation
    return std::abs(gridState->x - goalX_) + std::abs(gridState->y - goalY_);
}

void GridProblem::toggleWall(int x, int y) {
    if (y >= 0 && y < map_.size() && x >= 0 && x < map_[y].size()) {
        char& cell = map_[y][x];
        if (cell != 'S' && cell != 'G') {
            cell = (cell == '#') ? '.' : '#';
        }
    }
}

} // namespace ai_search
