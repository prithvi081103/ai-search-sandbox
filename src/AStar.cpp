#include "../include/AStar.h"
#include <queue>
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <vector>

namespace ai_search {

// Internal node struct for A* Search
struct AStarNode {
    std::shared_ptr<State> state;
    std::shared_ptr<AStarNode> parent;
    double gCost; // Cost from start to this node
    double fCost; // gCost + hCost (heuristic)

    AStarNode(std::shared_ptr<State> s, std::shared_ptr<AStarNode> p, double g, double f)
        : state(std::move(s)), parent(std::move(p)), gCost(g), fCost(f) {}
};

// Comparator for the priority queue (min-heap based on fCost)
struct CompareF {
    bool operator()(const std::shared_ptr<AStarNode>& a, const std::shared_ptr<AStarNode>& b) const {
        return a->fCost > b->fCost; // > for min-heap
    }
};

SearchResult AStar::solve(const SearchProblem& problem) {
    SearchResult result;
    auto startTime = std::chrono::high_resolution_clock::now();

    auto startState = problem.getStartState();
    if (!startState) {
        return result;
    }

    // Min-heap to prioritize the lowest f_cost
    std::priority_queue<std::shared_ptr<AStarNode>, std::vector<std::shared_ptr<AStarNode>>, CompareF> frontier;
    
    // Tracks the best g-cost to each state
    std::unordered_map<std::string, double> bestGCost; 

    double startH = problem.getHeuristic(startState);
    auto startNode = std::make_shared<AStarNode>(startState, nullptr, 0.0, startH);
    
    frontier.push(startNode);
    bestGCost[startState->getHash()] = 0.0;

    std::shared_ptr<AStarNode> goalNode = nullptr;

    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();

        // Stale node check: if we've already found a better path to this state since this node was pushed, skip it.
        // This is necessary because std::priority_queue doesn't support decrease-key operations natively.
        if (bestGCost.find(current->state->getHash()) != bestGCost.end() && 
            current->gCost > bestGCost[current->state->getHash()]) {
            continue;
        }

        result.nodesExpanded++;
        result.explorationHistory.push_back(current->state);

        if (problem.isGoalState(current->state)) {
            goalNode = current;
            result.success = true;
            break;
        }

        auto successors = problem.getSuccessors(current->state);
        for (const auto& action : successors) {
            double newGCost = current->gCost + action.cost;
            std::string hash = action.nextState->getHash();

            // If we found a strictly better path to this state, or it's completely unvisited
            if (bestGCost.find(hash) == bestGCost.end() || newGCost < bestGCost[hash]) {
                bestGCost[hash] = newGCost;
                double hCost = problem.getHeuristic(action.nextState);
                
                auto childNode = std::make_shared<AStarNode>(
                    action.nextState, 
                    current, 
                    newGCost, 
                    newGCost + hCost
                );
                
                frontier.push(childNode);
            }
        }
    }

    if (goalNode) {
        result.totalCost = goalNode->gCost;
        auto curr = goalNode;
        while (curr) {
            result.path.push_back(curr->state);
            curr = curr->parent;
        }
        std::reverse(result.path.begin(), result.path.end());
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    result.executionTimeMs = duration.count();

    return result;
}

} // namespace ai_search
