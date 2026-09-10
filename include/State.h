#pragma once

#include <string>

namespace ai_search {

/**
 * @brief Abstract base class representing a state in the search space.
 */
class State {
public:
    virtual ~State() = default;

    /**
     * @brief Get a unique string representation of the state for visited sets.
     * @return std::string hash
     */
    virtual std::string getHash() const = 0;

    /**
     * @brief Print the state to standard output (or other destination).
     */
    virtual void print() const = 0;
};

} // namespace ai_search
