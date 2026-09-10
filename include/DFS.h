#pragma once

#include "SearchAlgorithm.h"

namespace ai_search {

/**
 * @brief Depth-First Search (DFS) implementation of the SearchAlgorithm.
 * Uses a LIFO stack to explore nodes deeply before backtracking.
 */
class DFS : public SearchAlgorithm {
public:
    SearchResult solve(const SearchProblem& problem) override;
};

} // namespace ai_search
