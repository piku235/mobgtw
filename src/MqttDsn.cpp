#include "jungi/mobgtw/MqttDsn.h"

#include <regex>
#include <unordered_map>

namespace jungi::mobgtw {

std::optional<MqttDsn> MqttDsn::from(const std::string& dsn)
{
    std::regex uri(R"(^(mqtt(?:s)?)://(?:(.+?):(.+?)@)?([^:/?]+)(?::(\d+))?(?:\?([^#]+))?$)");
    std::smatch umatches;

    if (!std::regex_match(dsn, umatches, uri)) {
        return std::nullopt;
    }

    auto queryString = umatches[6].str();

    std::regex query(R"((\w+)=([^&]+))");
    std::unordered_map<std::string, std::string> queryParams;
    std::smatch qmatches;

    std::string::const_iterator start(queryString.cbegin());
    while (std::regex_search(start, queryString.cend(), qmatches, query)) {
        queryParams[qmatches[1]] = qmatches[2];
        start = qmatches.suffix().first;
    }

    std::optional<std::string> cacert;
    std::optional<bool> verify;

    {
        auto r = queryParams.find("cacert");
        if (r != queryParams.end()) {
            cacert = r->second;
        }
    }

    {
        auto r = queryParams.find("verify");
        if (r != queryParams.end()) {
            verify = "true" == r->second ? std::optional(true) : ("false" == r->second ? std::optional(false) : std::nullopt);
        }
    }

    return MqttDsn {
        .secure = "mqtts" == umatches[1].str(),
        .username = umatches[2].str().empty() ? std::nullopt : std::optional(umatches[2].str()),
        .password = umatches[3].str().empty() ? std::nullopt : std::optional(umatches[3].str()),
        .host = umatches[4].str(),
        .port = umatches[5].str().empty() ? std::nullopt : std::optional(static_cast<uint16_t>(std::stoi(umatches[5].str()))),
        .cacert = std::move(cacert),
        .verify = verify,
    };
}

}
