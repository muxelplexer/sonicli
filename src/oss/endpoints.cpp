#include "oss/endpoints.hpp"
#include "cpr/api.h"
#include "cpr/multiperform.h"
#include "cpr/session.h"
#include "oss/data/subsonic_response.hpp"
#include <algorithm>
#include <chrono>
#include <cpr/cpr.h>
#include <cpr/response.h>
#include <cpr/timeout.h>
#include <iostream>
#include <iterator>
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
        params.Add({ { "type", "newest" }, { "size", "500" } });
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
        sessions.reserve(albums_list.size());
        cpr::MultiPerform multi {};

        std::ranges::for_each(albums_list,
                              [&config, &uri, &sessions](const auto& album)
                              {
                                  auto session { std::make_shared<cpr::Session>() };
                                  auto params { *config.parameters };
                                  params.Add({
                                      { "id", album.id },
                                  });
                                  session->SetUrl(uri);
                                  session->SetParameters(params);
                                  sessions.emplace_back(std::move(session));
                              });

        for (auto& session : sessions)
        {
            multi.AddSession(session);
        }

        auto time2{std::chrono::steady_clock::now()};

        std::shared_ptr<cpr::Session> session = std::make_shared<cpr::Session>();
        std::vector<cpr::AsyncResponse> async_responses{};
        std::ranges::for_each(albums_list,
                [&async_responses, &uri, &config](const auto& album){
                    auto params { *config.parameters };
                    params.Add({
                        { "id", album.id },
                    });
                    static constexpr int32_t timeout_ms{500};
                    async_responses.emplace_back(cpr::GetAsync(uri, params, cpr::Timeout{timeout_ms}));
                }
        );
        std::vector<cpr::Response> responses{};
        std::ranges::transform(async_responses, std::back_inserter(responses), [](auto& album){
            return album.get();
        });

        auto time2e{std::chrono::steady_clock::now()};
        std::chrono::duration<double> diff2{time2e - time2};
        std::cerr << std::format("Took2: {}s\n", diff2.count());


        std::vector<data::album_response> song_responses {};
        std::ranges::transform(std::views::filter(responses,
                                                  [](const auto& res)
                                                  {
                                                      if (res.error)
                                                      {
                                                          std::cerr << "ERROR: " << res.error.message << "\n";
                                                      }
                                                      return !res.error;
                                                  }),
                               std::back_inserter(song_responses),
                               [](const auto& res)
                               {
                                   const auto body(json::parse(res.text));
                                   return body.at("subsonic-response").template get<oss::data::album_response>();
                               });
        return song_responses;
    }
} // namespace oss
