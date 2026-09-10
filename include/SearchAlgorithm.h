#pragma once

#include <memory>
#include <vector>
#include "State.h"
#include "SearchProblem.h"

namespace ai_search {

/**
 * @brief Represents the result of a search algorithm execution.
 */
struct SearchResult {
    std::vector<std::shared_ptr<State>> path;
    std::vector<std::shared_ptr<State>> explorationHistory;
    size_t nodesExpanded{0};
    double totalCost{0.0};
    double executionTimeMs{0.0};
    bool success{false};
};

/**
 * @brief Abstract base class for search algorithms (e.g., A*, BFS, DFS).
 */
class SearchAlgorithm {
public:
    virtual ~SearchAlgorithm() = default;

    /**
     * @brief Solves the given search problem and returns the result.
     * @param problem The problem to solve
     * @return SearchResult containing the path and search statistics
     */
    virtual SearchResult solve(const SearchProblem& problem) = 0;
};

} // namespace ai_search
