#include "../include/MDFBB.h"
#include <chrono>
#include <limits>

namespace ai_search {

void MDFBB::dfbb(std::shared_ptr<State> state, double gCost, std::vector<std::shared_ptr<State>>& currentPath, const SearchProblem& problem, std::unordered_map<std::string, double>& memory) {
    nodesExpanded_++;
    explorationHistory_.push_back(state);

    // Calculate fCost = gCost + hCost
    double fCost = gCost + problem.getHeuristic(state);
    
    // Branch and Bound pruning
    if (fCost >= bestCost_) {
        return;
    }

    // Check if we reached the goal with a strictly better cost (implied by the bound above)
    if (problem.isGoalState(state)) {
        bestCost_ = gCost;
        bestPath_ = currentPath;
        return;
    }

    std::string hash = state->getHash();

    // Memory Bound pruning: if we have visited this state with an equal or better gCost, prune
    auto it = memory.find(hash);
    if (it != memory.end() && it->second <= gCost) {
        return;
    }
    
    // Update the map with the current strictly better gCost
    memory[hash] = gCost;

    // Explore successors recursively
    auto successors = problem.getSuccessors(state);
    for (const auto& action : successors) {
        currentPath.push_back(action.nextState);
        
        dfbb(action.nextState, gCost + action.cost, currentPath, problem, memory);
        
        // Backtrack path
        currentPath.pop_back();
    }
}

SearchResult MDFBB::solve(const SearchProblem& problem) {
    SearchResult result;
    auto startTime = std::chrono::high_resolution_clock::now();

    auto startState = problem.getStartState();
    if (!startState) {
        return result;
    }

    bestCost_ = std::numeric_limits<double>::infinity();
    bestPath_.clear();
    nodesExpanded_ = 0;
    explorationHistory_.clear();

    std::unordered_map<std::string, double> memory;
    std::vector<std::shared_ptr<State>> currentPath;
    currentPath.push_back(startState);

    dfbb(startState, 0.0, currentPath, problem, memory);

    result.nodesExpanded = nodesExpanded_;
    result.explorationHistory = explorationHistory_;
    
    if (bestCost_ != std::numeric_limits<double>::infinity()) {
        result.success = true;
        result.totalCost = bestCost_;
        result.path = bestPath_;
    } else {
        result.success = false;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    result.executionTimeMs = duration.count();

    return result;
}

} // namespace ai_search
