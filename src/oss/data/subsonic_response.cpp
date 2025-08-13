#include "oss/data/subsonic_response.hpp"

namespace oss::data
{
    //
    // subsonic_error
    //

    void to_json(json& j, const subsonic_error& err)
    {
        j = json {
            { "code", err.code },
            { "message", err.message },
        };
    }

    void from_json(const json& j, subsonic_error& err)
    {
        j.at("code").get_to(err.code);
        if (j.contains("message"))
            j.at("message").get_to(err.message);
    }

    //
    // subsonic_response
    //

    void to_json(json& j, const subsonic_response& p)
    {
        j = json {
            { "status", p.status },
            { "version", p.version },
            { "type", p.type },
            { "serverVersion", p.serverVersion },
            { "openSubsonic", p.openSubsonic },
            { "error", p.error },
        };
    }
    void from_json(const json& j, subsonic_response& p)
    {
        j.at("status").get_to(p.status);
        j.at("version").get_to(p.version);
        j.at("type").get_to(p.type);
        j.at("serverVersion").get_to(p.serverVersion);
        j.at("openSubsonic").get_to(p.openSubsonic);
        if (j.contains("error"))
            j.at("error").get_to(p.error);
    }

    //
    // music_folder
    //

    void to_json(json& j, const music_folder& m)
    {
        j = json {
            { "id", m.id },
            { "name", m.name },
        };
    }
    void from_json(const json& j, music_folder& m)
    {
        j.at("id").get_to(m.id);
        j.at("name").get_to(m.name);
    }

    //
    // music_folder_response
    //

    void from_json(const json& j, music_folder_response& m)
    {
        nlohmann::from_json(j, static_cast<subsonic_response&>(m));
        if (j.contains("musicFolders"))
            j.at("musicFolders").at("musicFolder").get_to(m.music_folders);
    }

    void from_json(const json& j, music_track& m)
    {
        j.at("id").get_to(m.id);
        j.at("title").get_to(m.title);
    }

    void from_json(const json& j, album_list& a)
    {
        j.at("album").get_to(a.album);
    }

    void from_json(const json& j, album_list_response& a)
    {
        nlohmann::from_json(j, static_cast<subsonic_response&>(a));
        if (j.contains("albumList"))
            j.at("albumList").get_to(a.album_list);
    }

    void from_json(const json& j, album_id3& a)
    {
        j.at("song").get_to(a.children);
    }

    void from_json(const json& j, album_response& a)
    {
        nlohmann::from_json(j, static_cast<subsonic_response&>(a));
        if (j.contains("album"))
            j.at("album").get_to(a.album);
    }
} // namespace oss::data
