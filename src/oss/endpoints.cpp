#include "oss/endpoints.hpp"
#include "cpr/api.h"
#include "cpr/multiperform.h"
#include "cpr/session.h"
#include "oss/data/subsonic_response.hpp"
#include <cpr/cpr.h>
#include <iostream>
import nlohmann.json;

namespace oss
{
    using nlohmann::json;
    std::optional<data::subsonic_response> ping(const server_config& config)
    try
    {
        const auto res { cpr::Get(cpr::Url(config.url_string + "/rest/ping.view"), *config.parameters) };
        if (res.error)
        {
            return std::nullopt;
        }
        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::subsonic_response>();
    }
    catch (const std::exception& ex)
    {
        return std::nullopt;
    }

    std::optional<data::music_folder_response> getMusicFolders(const server_config& config)
    {
        const auto res { cpr::Get(cpr::Url(config.url_string + "/rest/getMusicFolders.view"), *config.parameters) };
        if (res.error)
        {
            return std::nullopt;
        }
        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::music_folder_response>();
    }

    std::optional<data::album_list_response> getAlbumList(const server_config& config)
    {
        auto params { *config.parameters };
        params.Add({ { "type", "newest" }, { "size", "100" } });
        const auto res { cpr::Get(cpr::Url(config.url_string + "/rest/getAlbumList.view"), params) };
        if (res.error)
        {
            return std::nullopt;
        }
        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::album_list_response>();
    }

    std::optional<data::album_response> getAlbum(const server_config& config, const std::string& album_id)
    {
        auto params { *config.parameters };
        params.Add({
            { "id", album_id },
        });
        const auto res { cpr::Get(cpr::Url(config.url_string + "/rest/getAlbum.view"), params) };
        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::album_response>();
    }

    std::optional<std::vector<data::album_response>> getAlbum(const server_config& config,
                                                              const std::vector<data::music_track>& albums_list)
    {
        cpr::Url uri(config.url_string + "/rest/getAlbum.view");
        std::vector<std::shared_ptr<cpr::Session>> sessions {};
        cpr::MultiPerform multi {};

        for (const auto& album : albums_list)
        {
            auto session { std::make_shared<cpr::Session>() };
            auto params { *config.parameters };
            params.Add({
                { "id", album.id },
            });
            session->SetUrl(uri);
            session->SetParameters(params);
            sessions.emplace_back(std::move(session));
        }

        for (auto& session : sessions)
        {
            multi.AddSession(session);
        }

        const auto responses { multi.Get() };
        std::vector<data::album_response> song_responses {};
        for (const auto& res : responses)
        {
            if (res.error)
            {
                std::cout << std::format("FETCH: {}\n", res.error.message);
                continue;
            }

            const auto body(json::parse(res.text));
            song_responses.emplace_back(body.at("subsonic-response").template get<oss::data::album_response>());
        }
        return song_responses;
    }
} // namespace oss
