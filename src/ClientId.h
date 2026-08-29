#pragma once

#include "jungi/mobgtw/Envelope.h"

#include <string>

namespace jungi::mobgtw {

class [[nodiscard]] ClientId {
public:
    using value_t = decltype(Envelope::clientId);

    static ClientId unique();
    static ClientId from(value_t value);

    [[nodiscard]] const value_t& value() const;
    [[nodiscard]] std::string toHex() const;

private:
    value_t mValue;

    ClientId(value_t value);
};

}
