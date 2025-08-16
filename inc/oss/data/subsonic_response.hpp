#pragma once

#include <optional>
#include <string>

#include <nlohmann/json.hpp>

namespace oss::data
{
    using nlohmann::json;
    struct subsonic_error
    {
        int code;
        std::optional<std::string> message;

        friend void to_json(json& obj, const subsonic_error& err);
        friend void from_json(const json& obj, subsonic_error& err);
    };

    struct subsonic_response
    {
        std::string status;
        std::string version;
        std::string type;
        std::string serverVersion;
        bool openSubsonic { false };
        std::optional<subsonic_error> error;

        friend void to_json(json& obj, const subsonic_response& res);
        friend void from_json(const json& obj, subsonic_response& res);
    };

    struct music_folder
    {
        int id;
        std::string name;

        friend void to_json(json& obj, const music_folder& mfolder);
        friend void from_json(const json& obj, music_folder& mfolder);
    };

    struct music_folder_response : public subsonic_response
    {
        std::optional<std::vector<music_folder>> music_folders;
        friend void from_json(const json& obj, music_folder_response& res);
    };

    struct music_track
    {
        std::string id;
        std::string title;
        friend void from_json(const json& obj, music_track& track);
    };

    struct album_list
    {
        std::vector<music_track> album;
        friend void from_json(const json& obj, album_list& album);
    };

    struct album_list_response : public subsonic_response
    {
        std::optional<album_list> alist;
        friend void from_json(const json& obj, album_list_response& res);
    };

    struct album_id3
    {
        std::string id;
        std::string name;
        std::optional<std::vector<music_track>> children;
        friend void from_json(const json& obj, album_id3& aid);
    };

    struct album_response : public subsonic_response
    {
        std::optional<album_id3> album;
        friend void from_json(const json& obj, album_response& res);
    };

} // namespace oss::data
