#pragma once

#include "SearchAlgorithm.h"
#include <unordered_map>
#include <vector>
#include <string>

namespace ai_search {

/**
 * @brief Memory-Bounded Depth-First Branch and Bound (MDFBB).
 * Uses DFS but prunes branches whose f-cost exceeds the best known goal cost,
 * and uses a memory map to prune states visited with an equal or better g-cost.
 */
class MDFBB : public SearchAlgorithm {
private:
    double bestCost_;
    std::vector<std::shared_ptr<State>> bestPath_;
    size_t nodesExpanded_;
    std::vector<std::shared_ptr<State>> explorationHistory_;

    /**
     * @brief Recursive helper method for MDFBB.
     */
    void dfbb(std::shared_ptr<State> state, double gCost, std::vector<std::shared_ptr<State>>& currentPath, const SearchProblem& problem, std::unordered_map<std::string, double>& memory);

public:
    SearchResult solve(const SearchProblem& problem) override;
};

} // namespace ai_search
