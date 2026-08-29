#include "utils.h"

#include <arpa/inet.h>
#include <openssl/evp.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>

namespace jungi::mobgtw::crypto {

bytes timestamp2iv(const time_t timestamp)
{
    bytes iv(EVP_MAX_IV_LENGTH, '\0');
    auto bigEndianTimestamp = htonl(static_cast<uint32_t>(timestamp));
    auto timestampBytes = std::bit_cast<std::array<byte, sizeof(bigEndianTimestamp)>>(bigEndianTimestamp);

    std::copy(
        timestampBytes.begin(),
        timestampBytes.end(),
        iv.end() - timestampBytes.size()
    );

    return iv;
}

}
