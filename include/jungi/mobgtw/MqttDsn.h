#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace jungi::mobgtw {

struct [[nodiscard]] MqttDsn final {
    bool secure;
    std::optional<std::string> username;
    std::optional<std::string> password;
    std::string host;
    std::optional<uint16_t> port;
    std::optional<std::string> cacert;
    std::optional<bool> verify;

    [[nodiscard]] static std::optional<MqttDsn> from(const std::string& dsn);
};

}
