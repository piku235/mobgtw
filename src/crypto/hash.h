#pragma once

#include "bytes.h"
#include <string>

namespace jungi::mobgtw::crypto {

[[nodiscard]] bytes sha256(const std::string& text);

}
