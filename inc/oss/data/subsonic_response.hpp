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
        inline friend void to_json(json& j, const subsonic_error& err)
        {
            j = json{
                {"code", err.code},
                {"message", err.message},
            };
        }

        inline friend void from_json(const json& j, subsonic_error& err)
        {
            j.at("code").get_to(err.code);
            if (j.contains("message"))
                j.at("message").get_to(err.message);
        }
    };

    struct subsonic_response
    {
        std::string status{""};
        std::string version{""};
        std::string type{""};
        std::string serverVersion{""};
        bool openSubsonic{false};
        std::optional<subsonic_error> error;

        inline friend void to_json(json& j, const subsonic_response& p) {
            j = json{
                {"status", p.status},
                {"version", p.version},
                {"type", p.type},
                {"serverVersion", p.serverVersion},
                {"openSubsonic", p.openSubsonic},
                {"error", p.error}
            };
        }

        inline friend void from_json(const json& j, subsonic_response& p) {
            j.at("status").get_to(p.status);
            j.at("version").get_to(p.version);
            j.at("type").get_to(p.type);
            j.at("serverVersion").get_to(p.serverVersion);
            j.at("openSubsonic").get_to(p.openSubsonic);
            if (j.contains("error"))
                j.at("error").get_to(p.error);
        }
    };

    struct music_folder
    {
        int id;
        std::string name{};
        inline friend void to_json(json& j, const music_folder& m)
        {
            j = json{
                {"id", m.id},
                {"name", m.name}
            };
        }
        inline friend void from_json(const json& j, music_folder& m)
        {
            j.at("id").get_to(m.id);
            j.at("name").get_to(m.name);
        }
    };

    struct music_folder_response : public subsonic_response
    {
        std::optional<std::vector<music_folder>> music_folders;
        inline friend void from_json(const json& j, music_folder_response& m)
        {
            nlohmann::from_json(j, static_cast<subsonic_response&>(m));
            if (j.contains("musicFolders"))
                j.at("musicFolders").at("musicFolder").get_to(m.music_folders);
        }
    };
}
