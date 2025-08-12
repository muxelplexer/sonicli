#pragma once
#include <optional>
#include <string>

#include "oss/data/subsonic_response.hpp"
#include "oss/server_config.hpp"

namespace oss
{
    std::optional<data::subsonic_response> ping(const server_config& config);
    std::optional<data::music_folder_response> getMusicFolders(const server_config& config);
    std::optional<data::subsonic_response> getMusicDirectory(const server_config& config, const std::string& id);
}
