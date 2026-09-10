#pragma once

#include <vector>
#include <string>
#include <memory>
#include "State.h"
#include "SearchProblem.h"

namespace ai_search {

/**
 * @brief Represents a state in a 2D grid problem.
 */
class GridState : public State {
public:
    int x;
    int y;

    GridState(int x, int y);

    std::string getHash() const override;
    void print() const override;
};

/**
 * @brief Concrete implementation of a SearchProblem for a 2D grid map.
 */
class GridProblem : public SearchProblem {
private:
    std::vector<std::string> map_;
    int goalX_{-1};
    int goalY_{-1};

public:
    /**
     * @brief Constructs a GridProblem with the given 2D map.
     * @param map Vector of strings representing the grid ('S'=start, 'G'=goal, '#'=wall, '.'=open)
     */
    explicit GridProblem(const std::vector<std::string>& map);

    std::shared_ptr<State> getStartState() const override;
    bool isGoalState(const std::shared_ptr<State>& state) const override;
    std::vector<Action> getSuccessors(const std::shared_ptr<State>& state) const override;
    double getHeuristic(const std::shared_ptr<State>& state) const override;

    const std::vector<std::string>& getMap() const { return map_; }
    void toggleWall(int x, int y);
};

} // namespace ai_search
