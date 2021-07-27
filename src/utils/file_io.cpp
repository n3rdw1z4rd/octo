#include "file_io.hpp"

#include <spdlog/spdlog.h>
#include <fstream>

namespace octo
{
    nlohmann::json loadJsonFile(const std::string &path)
    {
        std::ifstream file(path.c_str());

        if (!file.good())
        {
            spdlog::warn("loadJsonFile: cannot find '{}'", path);
            return nullptr;
        }

        nlohmann::json json = nlohmann::json::parse(file);
        file.close();

        spdlog::debug("loadJsonFile: loaded '{}': {}", path, json.dump(2));

        return json;
    }

    bool saveJsonFile(const std::string &path, nlohmann::json json)
    {
        if (path == "")
        {
            spdlog::warn("saveJsonFile: missing valid path");
            return false;
        }

        std::ofstream *h = new std::ofstream(path.c_str(), std::ios::trunc);
        (*h) << json.dump(4);
        h->close();

        return true;
    }
}