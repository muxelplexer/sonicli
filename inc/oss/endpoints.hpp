#pragma once
#include <optional>

#include "oss/data/subsonic_response.hpp"
#include "oss/server_config.hpp"

namespace oss
{
    std::optional<data::subsonic_response> ping(const server_config& config);
    std::optional<data::music_folder_response> getMusicFolders(const server_config& config);
    std::optional<data::album_list_response> getAlbumList(const server_config& config);
    std::optional<data::album_response> getAlbum(const server_config& config, const std::string& album_id);
}
