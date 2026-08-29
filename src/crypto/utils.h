#pragma once

#include "bytes.h"
#include <ctime>

namespace jungi::mobgtw::crypto {

[[nodiscard]] bytes timestamp2iv(time_t timestamp);

}
