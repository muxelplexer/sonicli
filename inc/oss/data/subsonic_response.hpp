#pragma once

#include <optional>
#include <string>
#include <nlohmann/json.hpp>

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
        std::string status{""};
        std::string version{""};
        std::string type{""};
        std::string serverVersion{""};
        bool openSubsonic{false};
        std::optional<subsonic_error> error;

        friend void to_json(json& j, const subsonic_response& p);
        friend void from_json(const json& j, subsonic_response& p);
    };

    struct music_folder
    {
        int id;
        std::string name{};

        friend void to_json(json& j, const music_folder& m);
        friend void from_json(const json& j, music_folder& m);
    };

    struct music_folder_response : public subsonic_response
    {
        std::optional<std::vector<music_folder>> music_folders;
        friend void from_json(const json& j, music_folder_response& m);
    };
}
