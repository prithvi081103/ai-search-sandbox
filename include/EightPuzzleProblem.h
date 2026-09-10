#pragma once

#include "State.h"
#include "SearchProblem.h"
#include <vector>
#include <string>
#include <memory>

namespace ai_search {

/**
 * @brief Represents a state in the 8-Puzzle game.
 */
class EightPuzzleState : public State {
public:
    std::vector<int> board;

    explicit EightPuzzleState(const std::vector<int>& initialBoard);

    std::string getHash() const override;
    void print() const override;
};

/**
 * @brief Concrete implementation of a SearchProblem for the 8-Puzzle game.
 */
class EightPuzzleProblem : public SearchProblem {
private:
    std::shared_ptr<EightPuzzleState> startState_;

public:
    explicit EightPuzzleProblem(const std::vector<int>& initialBoard);

    std::shared_ptr<State> getStartState() const override;
    bool isGoalState(const std::shared_ptr<State>& state) const override;
    std::vector<Action> getSuccessors(const std::shared_ptr<State>& state) const override;
    double getHeuristic(const std::shared_ptr<State>& state) const override;

    void manuallySwapTile(int clickedRow, int clickedCol);
    void randomizeBoard();
};

} // namespace ai_search
