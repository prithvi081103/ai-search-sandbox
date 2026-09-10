#include "../include/BFS.h"
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>

namespace ai_search {

// Internal node struct to keep track of the search tree (parents and cost)
struct SearchNode {
    std::shared_ptr<State> state;
    std::shared_ptr<SearchNode> parent;
    double cost;

    SearchNode(std::shared_ptr<State> s, std::shared_ptr<SearchNode> p, double c)
        : state(std::move(s)), parent(std::move(p)), cost(c) {}
};

SearchResult BFS::solve(const SearchProblem& problem) {
    SearchResult result;
    
    // Start the timer
    auto startTime = std::chrono::high_resolution_clock::now();

    auto startState = problem.getStartState();
    if (!startState) {
        return result; // Empty result if no start state
    }

    std::queue<std::shared_ptr<SearchNode>> frontier;
    std::unordered_set<std::string> visited;

    auto startNode = std::make_shared<SearchNode>(startState, nullptr, 0.0);
    frontier.push(startNode);
    visited.insert(startState->getHash());

    std::shared_ptr<SearchNode> goalNode = nullptr;

    // Standard BFS Loop
    while (!frontier.empty()) {
        auto current = frontier.front();
        frontier.pop();
        result.nodesExpanded++;
        result.explorationHistory.push_back(current->state);

        // Check if we reached the goal
        if (problem.isGoalState(current->state)) {
            goalNode = current;
            result.success = true;
            break;
        }

        // Expand node and check successors
        auto successors = problem.getSuccessors(current->state);
        for (const auto& action : successors) {
            std::string hash = action.nextState->getHash();
            
            // If the state hasn't been visited yet, add it to frontier and visited set
            if (visited.find(hash) == visited.end()) {
                visited.insert(hash);
                
                auto childNode = std::make_shared<SearchNode>(
                    action.nextState, 
                    current, 
                    current->cost + action.cost
                );
                
                frontier.push(childNode);
            }
        }
    }

    // Reconstruct the path if we found the goal
    if (goalNode) {
        result.totalCost = goalNode->cost;
        auto curr = goalNode;
        while (curr) {
            result.path.push_back(curr->state);
            curr = curr->parent;
        }
        // The path is currently from Goal -> Start, reverse it to Start -> Goal
        std::reverse(result.path.begin(), result.path.end());
    }

    // Stop the timer and compute execution time in milliseconds
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    result.executionTimeMs = duration.count();

    return result;
}

} // namespace ai_search
