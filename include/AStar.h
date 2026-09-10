#pragma once

#include "SearchAlgorithm.h"

namespace ai_search {

/**
 * @brief A* Search implementation of the SearchAlgorithm.
 * Uses a priority queue to explore nodes with the lowest f = g + h cost.
 */
class AStar : public SearchAlgorithm {
public:
    SearchResult solve(const SearchProblem& problem) override;
};

} // namespace ai_search
