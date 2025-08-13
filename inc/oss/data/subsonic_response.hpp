#pragma once

#include <nlohmann/json.hpp>
#include <optional>
#include <string>

import nlohmann.json;
namespace oss::data
{
    using nlohmann::json;
    struct subsonic_error
    {
        int code;
        std::optional<std::string> message;

        friend void to_json(json& j, const subsonic_error& err);
        friend void from_json(const json& j, subsonic_error& err);
    };

    struct subsonic_response
    {
        std::string status { "" };
        std::string version { "" };
        std::string type { "" };
        std::string serverVersion { "" };
        bool openSubsonic { false };
        std::optional<subsonic_error> error;

        friend void to_json(json& j, const subsonic_response& p);
        friend void from_json(const json& j, subsonic_response& p);
    };

    struct music_folder
    {
        int id;
        std::string name {};

        friend void to_json(json& j, const music_folder& m);
        friend void from_json(const json& j, music_folder& m);
    };

    struct music_folder_response : public subsonic_response
    {
        std::optional<std::vector<music_folder>> music_folders;
        friend void from_json(const json& j, music_folder_response& m);
    };

    struct music_track
    {
        std::string id {};
        std::string title {};
        friend void from_json(const json& j, music_track& m);
    };

    struct album_list
    {
        std::vector<music_track> album;
        friend void from_json(const json& j, album_list& a);
    };

    struct album_list_response : public subsonic_response
    {
        std::optional<album_list> album_list;
        friend void from_json(const json& j, album_list_response& a);
    };

    struct album_id3
    {
        std::string id {};
        std::string name {};
        std::optional<std::vector<music_track>> children {};
        friend void from_json(const json& j, album_id3& a);
    };

    struct album_response : public subsonic_response
    {
        std::optional<album_id3> album {};
        friend void from_json(const json& j, album_response& a);
    };

} // namespace oss::data
