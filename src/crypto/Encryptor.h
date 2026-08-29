#pragma once

#include "bytes.h"

namespace jungi::mobgtw::crypto {

class Encryptor {
public:
    virtual ~Encryptor() = default;

    [[nodiscard]] virtual bytes encrypt(const bytes& plaintext, const bytes& key, const bytes& iv) const = 0;
    [[nodiscard]] virtual bytes decrypt(const bytes& ciphertext, const bytes& key, const bytes& iv) const = 0;
};

}
