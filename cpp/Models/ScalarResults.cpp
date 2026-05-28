#include "ScalarResults.h"
#include <mutex>

ScalarResults::~ScalarResults() = default;

std::optional<ScalarResult> ScalarResults::operator[](const std::string& tradeId) const {
    if (!containsTrade(tradeId)) {
        return std::nullopt;
    }

    auto resultIt = results_.find(tradeId);
    auto errorIt = errors_.find(tradeId);

    if (resultIt == results_.end() && errorIt == errors_.end()) {
        return std::nullopt;
    }

    std::optional<double> priceResult = resultIt != results_.end() ? resultIt->second : std::optional<double>{};
    std::optional<std::string> error = errorIt != errors_.end() ? errorIt->second : std::optional<std::string>{};
    return ScalarResult(tradeId, priceResult, error);

}

bool ScalarResults::containsTrade(const std::string& tradeId) const {
    return results_.find(tradeId) != results_.end() || errors_.find(tradeId) != errors_.end();
}

void ScalarResults::addResult(const std::string& tradeId, double result) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!containsTrade(tradeId)) {
        keys_.push_back(tradeId);
    }
    results_[tradeId] = result;
}

void ScalarResults::addError(const std::string& tradeId, const std::string& error) {
    std::lock_guard<std::mutex> lock(mutex_);
    // If containsTrade is ever called without a lock, there will be a race condition.
    if (!containsTrade(tradeId)) {
        keys_.push_back(tradeId);
    }
    errors_[tradeId] = error;
}

ScalarResults::Iterator& ScalarResults::Iterator::operator++() {
    index_++;
    return *this;
}

ScalarResult ScalarResults::Iterator::operator*() const {
    std::string currentTradeId = parent_->keys_[index_];
    return parent_->operator[](currentTradeId).value();
}

bool ScalarResults::Iterator::operator!=(const Iterator& other) const {
    return index_ != other.index_;
}

ScalarResults::Iterator ScalarResults::begin() const {
    return Iterator(this, 0);
}

ScalarResults::Iterator ScalarResults::end() const {
    return Iterator(this, keys_.size());
}
