#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace jungi::mobgtw {

struct [[nodiscard]] Envelope final {
    struct ResponseStatus {
        static constexpr uint8_t Success = 0;
        static constexpr uint8_t InvalidSession = 1;
    };

    uint8_t messageType;
    uint32_t timestamp;
    std::array<uint8_t, 6> clientId;
    uint8_t platform;
    uint8_t responseStatus;
    std::vector<uint8_t> messageBody;

    [[nodiscard]] static std::optional<Envelope> deserialize(const uint8_t* payload, uint32_t size);
    [[nodiscard]] std::vector<uint8_t> serialize() const;

    [[nodiscard]] uint32_t size() const;
    bool operator==(const Envelope& other) const = default;
};

}
