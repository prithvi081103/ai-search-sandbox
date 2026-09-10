#include "../include/IDDFS.h"
#include <chrono>
#include <algorithm>

namespace ai_search {

bool IDDFS::DLS(std::shared_ptr<State> state, int depth, const SearchProblem& problem, std::unordered_set<std::string>& visited, SearchResult& result) {
    result.nodesExpanded++;
    result.explorationHistory.push_back(state);

    // Base case: if depth limit is reached, check if we found the goal
    if (depth == 0) {
        if (problem.isGoalState(state)) {
            result.path.push_back(state);
            return true;
        }
        return false;
    }

    // Recursive step: explore successors if we have depth > 0 remaining
    if (depth > 0) {
        auto successors = problem.getSuccessors(state);
        for (const auto& action : successors) {
            std::string hash = action.nextState->getHash();

            // Prevent cycles within the current DFS path branch
            if (visited.find(hash) == visited.end()) {
                visited.insert(hash);

                if (DLS(action.nextState, depth - 1, problem, visited, result)) {
                    // Backtrack and build the successful path and accumulate cost
                    result.path.push_back(state);
                    result.totalCost += action.cost;
                    return true;
                }

                // Remove from visited so other branches can explore this node
                visited.erase(hash);
            }
        }
    }

    return false;
}

SearchResult IDDFS::solve(const SearchProblem& problem) {
    SearchResult result;
    auto startTime = std::chrono::high_resolution_clock::now();

    auto startState = problem.getStartState();
    if (!startState) {
        return result;
    }

    const int MAX_DEPTH = 1000; // Define a reasonable maximum depth

    for (int limit = 0; limit < MAX_DEPTH; ++limit) {
        std::unordered_set<std::string> visited;
        visited.insert(startState->getHash());

        // Perform Depth-Limited Search for this depth limit
        if (DLS(startState, limit, problem, visited, result)) {
            result.success = true;
            // The path was constructed Goal -> Start, reverse it
            std::reverse(result.path.begin(), result.path.end());
            break;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    result.executionTimeMs = duration.count();

    return result;
}

} // namespace ai_search
