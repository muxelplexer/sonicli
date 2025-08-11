#pragma once
#include "oss/server_config.hpp"
#include <optional>
#include <string>

namespace oss
{
    std::optional<std::string> ping(const server_config& config);
    std::optional<std::string> getMusicFolders(const server_config& config);
    std::optional<std::string> getMusicDirectory(const server_config& config, const std::string& id);
}
