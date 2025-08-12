#include "oss/endpoints.hpp"
#include "oss/data/subsonic_response.hpp"
#include <cpr/cpr.h>
import nlohmann.json;

namespace oss
{
    using nlohmann::json;
    std::optional<data::subsonic_response> ping(const server_config& config) try
    {
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/ping.view"), *config.parameters)};

        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::subsonic_response>();
    } catch (const std::exception& ex)
    {
        return std::nullopt;
    }

    std::optional<data::music_folder_response> getMusicFolders(const server_config& config)
    {
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getMusicFolders.view"), *config.parameters)};
        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::music_folder_response>();
    }

    std::optional<data::album_list_response> getAlbumList(const server_config& config)
    {
        auto params{*config.parameters};
        params.Add(
            {
                {"type", "newest"},
                {"size", "25"}
            }
        );
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getAlbumList.view"), params)};
        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::album_list_response>();
    }
}
