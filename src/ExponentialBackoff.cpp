#include "ExponentialBackoff.h"

#include <algorithm>
#include <cstdlib>

namespace jungi::mobgtw {

ExponentialBackoff::ExponentialBackoff(std::chrono::milliseconds baseDelay, std::chrono::milliseconds maxDelay)
    : mBaseDelay(baseDelay)
    , mDelay(baseDelay)
    , mMaxDelay(maxDelay)
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
