#include "../include/EightPuzzleProblem.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

namespace ai_search {

// --- EightPuzzleState ---

EightPuzzleState::EightPuzzleState(const std::vector<int>& initialBoard)
    : board(initialBoard) {}

std::string EightPuzzleState::getHash() const {
    std::stringstream ss;
    for (int val : board) {
        ss << val;
    }
    return ss.str(); // Example output: "012345678"
}

void EightPuzzleState::print() const {
    for (int i = 0; i < 9; ++i) {
        std::cout << board[i] << " ";
        if ((i + 1) % 3 == 0) {
            std::cout << "\n";
        }
    }
}

// --- EightPuzzleProblem ---

EightPuzzleProblem::EightPuzzleProblem(const std::vector<int>& initialBoard) {
    startState_ = std::make_shared<EightPuzzleState>(initialBoard);
}

std::shared_ptr<State> EightPuzzleProblem::getStartState() const {
    return startState_;
}

bool EightPuzzleProblem::isGoalState(const std::shared_ptr<State>& state) const {
    auto pState = std::dynamic_pointer_cast<EightPuzzleState>(state);
    if (!pState) return false;

    // The target is 0, 1, 2, 3, 4, 5, 6, 7, 8
    for (int i = 0; i < 9; ++i) {
        if (pState->board[i] != i) {
            return false;
        }
    }
    return true;
}

std::vector<Action> EightPuzzleProblem::getSuccessors(const std::shared_ptr<State>& state) const {
    std::vector<Action> successors;
    auto pState = std::dynamic_pointer_cast<EightPuzzleState>(state);
    if (!pState) return successors;

    // Find the empty tile (0)
    int zeroIdx = -1;
    for (int i = 0; i < 9; ++i) {
        if (pState->board[i] == 0) {
            zeroIdx = i;
            break;
        }
    }

    int r = zeroIdx / 3;
    int c = zeroIdx % 3;

    // Offsets for UP, DOWN, LEFT, RIGHT
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        // Ensure we are inside the 3x3 grid boundaries
        if (nr >= 0 && nr < 3 && nc >= 0 && nc < 3) {
            int newIdx = nr * 3 + nc;
            std::vector<int> newBoard = pState->board;
            
            // Swap 0 with the adjacent tile
            std::swap(newBoard[zeroIdx], newBoard[newIdx]);

            Action action;
            action.nextState = std::make_shared<EightPuzzleState>(newBoard);
            action.cost = 1.0; // Uniform cost
            successors.push_back(action);
        }
    }

    return successors;
}

double EightPuzzleProblem::getHeuristic(const std::shared_ptr<State>& state) const {
    auto pState = std::dynamic_pointer_cast<EightPuzzleState>(state);
    if (!pState) return 0.0;

    double heuristic = 0.0;
    for (int i = 0; i < 9; ++i) {
        int val = pState->board[i];
        if (val != 0) {
            // Current coordinates
            int cr = i / 3;
            int cc = i % 3;
            
            // Goal coordinates (since goal board is 0, 1, 2, ..., 8)
            int gr = val / 3;
            int gc = val % 3;
            
            // Add Manhattan distance for this tile
            heuristic += std::abs(cr - gr) + std::abs(cc - gc);
        }
    }
    return heuristic;
}

void EightPuzzleProblem::manuallySwapTile(int clickedRow, int clickedCol) {
    if (clickedRow < 0 || clickedRow >= 3 || clickedCol < 0 || clickedCol >= 3) return;

    int zeroIdx = -1;
    for (int i = 0; i < 9; ++i) {
        if (startState_->board[i] == 0) {
            zeroIdx = i;
            break;
        }
    }

    int zeroRow = zeroIdx / 3;
    int zeroCol = zeroIdx % 3;

    if (std::abs(zeroRow - clickedRow) + std::abs(zeroCol - clickedCol) == 1) {
        int clickedIdx = clickedRow * 3 + clickedCol;
        std::swap(startState_->board[zeroIdx], startState_->board[clickedIdx]);
    }
}

void EightPuzzleProblem::randomizeBoard() {
    std::vector<int> board = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int zeroIdx = 0;
    
    for (int i = 0; i < 100; ++i) {
        int r = zeroIdx / 3;
        int c = zeroIdx % 3;
        
        std::vector<int> validMoves;
        if (r > 0) validMoves.push_back((r - 1) * 3 + c);
        if (r < 2) validMoves.push_back((r + 1) * 3 + c);
        if (c > 0) validMoves.push_back(r * 3 + (c - 1));
        if (c < 2) validMoves.push_back(r * 3 + (c + 1));
        
        int randMove = validMoves[std::rand() % validMoves.size()];
        std::swap(board[zeroIdx], board[randMove]);
        zeroIdx = randMove;
    }
    
    startState_->board = board;
}

} // namespace ai_search
