#include "ExponentialBackoff.h"

#include <algorithm>
#include <cstdlib>

namespace jungi::mobgtw {

ExponentialBackoff::ExponentialBackoff(std::chrono::milliseconds baseDelay, std::chrono::milliseconds maxDelay, uint8_t factor)
    : mBaseDelay(baseDelay)
    , mDelay(baseDelay)
    , mMaxDelay(maxDelay)
    , mFactor(factor)
    , mGenerator(std::random_device {}())
{
}

void ExponentialBackoff::reset()
{
    mDelay = mBaseDelay;
}

std::chrono::milliseconds ExponentialBackoff::next()
{
    auto delay = mDelay;
    std::uniform_int_distribution<std::chrono::milliseconds::rep> rand(0, delay.count());

    delay = std::chrono::milliseconds(rand(mGenerator));
    mDelay = std::min(mDelay * 2, mMaxDelay);

    return delay;
}

}
