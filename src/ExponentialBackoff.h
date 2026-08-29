#pragma once

#include <chrono>
#include <random>
#include <cstdint>

namespace jungi::mobgtw {

class ExponentialBackoff final {
public:
    ExponentialBackoff(std::chrono::milliseconds baseDelay, std::chrono::milliseconds maxDelay, uint8_t factor = 2);

    void reset();
    [[nodiscard]] std::chrono::milliseconds next();

private:
    std::chrono::milliseconds mBaseDelay;
    std::chrono::milliseconds mDelay;
    std::chrono::milliseconds mMaxDelay;
    uint8_t mFactor;
    std::mt19937 mGenerator;
};

}
