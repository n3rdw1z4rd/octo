#pragma once

#include "../core/defines.hpp"
#include "../third_party/json.hpp"

namespace octo {
    nlohmann::json loadJsonFile(const std::string &path);
    bool saveJsonFile(const std::string &path, nlohmann::json json);
}