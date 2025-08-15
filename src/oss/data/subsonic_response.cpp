#include "oss/data/subsonic_response.hpp"

namespace oss::data
{
    //
    // subsonic_error
    //

    void to_json(json& obj, const subsonic_error& err)
    {
        obj = json {
            { "code", err.code },
            { "message", err.message },
        };
    }

    void from_json(const json& obj, subsonic_error& err)
    {
        obj.at("code").get_to(err.code);
        if (obj.contains("message"))
        {
            obj.at("message").get_to(err.message);
        }
    }

    //
    // subsonic_response
    //

    void to_json(json& obj, const subsonic_response& res)
    {
        obj = json {
            { "status", res.status },
            { "version", res.version },
            { "type", res.type },
            { "serverVersion", res.serverVersion },
            { "openSubsonic", res.openSubsonic },
            { "error", res.error },
        };
    }
    void from_json(const json& obj, subsonic_response& res)
    {
        obj.at("status").get_to(res.status);
        obj.at("version").get_to(res.version);
        obj.at("type").get_to(res.type);
        obj.at("serverVersion").get_to(res.serverVersion);
        obj.at("openSubsonic").get_to(res.openSubsonic);
        if (obj.contains("error"))
        {
            obj.at("error").get_to(res.error);
        }
    }

    //
    // music_folder
    //

    void to_json(json& obj, const music_folder& mfolder)
    {
        obj = json {
            { "id", mfolder.id },
            { "name", mfolder.name },
        };
    }
    void from_json(const json& obj, music_folder& mfolder)
    {
        obj.at("id").get_to(mfolder.id);
        obj.at("name").get_to(mfolder.name);
    }

    //
    // music_folder_response
    //

    void from_json(const json& obj, music_folder_response& res)
    {
        nlohmann::from_json(obj, static_cast<subsonic_response&>(res));
        if (obj.contains("musicFolders"))
        {
            obj.at("musicFolders").at("musicFolder").get_to(res.music_folders);
        }
    }

    void from_json(const json& obj, music_track& track)
    {
        obj.at("id").get_to(track.id);
        obj.at("title").get_to(track.title);
    }

    void from_json(const json& obj, album_list& album)
    {
        obj.at("album").get_to(album.album);
    }

    void from_json(const json& obj, album_list_response& res)
    {
        nlohmann::from_json(obj, static_cast<subsonic_response&>(res));
        if (obj.contains("albumList"))
        {
            obj.at("albumList").get_to(res.album_list);
        }
    }

    void from_json(const json& obj, album_id3& aid)
    {
        if (obj.contains("song"))
        {
            obj.at("song").get_to(aid.children);
        }
    }

    void from_json(const json& obj, album_response& res)
    {
        nlohmann::from_json(obj, static_cast<subsonic_response&>(res));
        if (obj.contains("album"))
        {
            obj.at("album").get_to(res.album);
        }
    }
} // namespace oss::data
