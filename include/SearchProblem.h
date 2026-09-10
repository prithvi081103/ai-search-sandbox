#pragma once

#include <memory>
#include <vector>
#include "State.h"

namespace ai_search {

/**
 * @brief Represents an action leading to a next state and the cost of that transition.
 */
struct Action {
    std::shared_ptr<State> nextState;
    double cost;
};

/**
 * @brief Abstract base class representing a search problem to be solved.
 */
class SearchProblem {
public:
    virtual ~SearchProblem() = default;

    /**
     * @brief Returns the start state for the search problem.
     * @return std::shared_ptr<State> start state
     */
    virtual std::shared_ptr<State> getStartState() const = 0;

    /**
     * @brief Checks if the given state is a goal state.
     * @param state The state to check
     * @return true if the state is a goal state, false otherwise
     */
    virtual bool isGoalState(const std::shared_ptr<State>& state) const = 0;

    /**
     * @brief Returns a list of valid actions from the given state.
     * @param state The state to expand
     * @return std::vector<Action> list of successors
     */
    virtual std::vector<Action> getSuccessors(const std::shared_ptr<State>& state) const = 0;

    /**
     * @brief Returns a heuristic estimate from the given state to the goal.
     * @param state The state to evaluate
     * @return double Heuristic cost (default 0.0 for uninformed search)
     */
    virtual double getHeuristic(const std::shared_ptr<State>& state) const {
        return 0.0;
    }
};

} // namespace ai_search
