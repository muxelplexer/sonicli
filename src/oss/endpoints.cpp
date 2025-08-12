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

    std::optional<data::subsonic_response> getMusicFolders(const server_config& config)
    {
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getMusicFolders.view"), *config.parameters)};
        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::subsonic_response>();
    }

    std::optional<data::subsonic_response> getMusicDirectory(const server_config& config, const std::string& id)
    {
        auto parameters{*config.parameters};
        parameters.Add({"id", id});
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getMusicDirectory.view"), parameters)};
        if (res.error)
        {
            return std::nullopt;
        }

        const auto body(json::parse(res.text));
        return body.at("subsonic-response").template get<oss::data::subsonic_response>();
    }
}
