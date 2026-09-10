#pragma once

#include "SearchAlgorithm.h"

namespace ai_search {

/**
 * @brief Breadth-First Search (BFS) implementation of the SearchAlgorithm.
 * Uses a FIFO queue to explore nodes level-by-level.
 */
class BFS : public SearchAlgorithm {
public:
    SearchResult solve(const SearchProblem& problem) override;
};

} // namespace ai_search
