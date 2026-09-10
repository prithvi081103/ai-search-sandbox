#pragma once

#include "SearchAlgorithm.h"
#include <unordered_set>
#include <string>

namespace ai_search {

/**
 * @brief Iterative Deepening Depth-First Search (IDDFS) implementation.
 * Combines the space-efficiency of DFS with the optimality of BFS.
 */
class IDDFS : public SearchAlgorithm {
private:
    /**
     * @brief Recursive Depth-Limited Search helper.
     */
    bool DLS(std::shared_ptr<State> state, int depth, const SearchProblem& problem, std::unordered_set<std::string>& visited, SearchResult& result);

public:
    SearchResult solve(const SearchProblem& problem) override;
};

} // namespace ai_search
