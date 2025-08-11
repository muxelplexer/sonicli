#include "oss/endpoints.hpp"
#include <cpr/cpr.h>
import nlohmann.json;

namespace oss
{
    using nlohmann::json;
    std::optional<std::string> ping(const server_config& config) try
    {
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/ping.view"), *config.parameters)};

        if (res.error)
        {
            return res.error.message;
        }

        const auto body(json::parse(res.text));
        const auto status = body.at("subsonic-response").at("status").get<std::string>();

        if (status == "ok")
            return std::nullopt;
        else 
            return body.at("subsonic-response").at("error").at("message").get<std::string>();
    } catch (const std::exception& ex)
    {
        return ex.what();
    }

    std::optional<std::string> getMusicFolders(const server_config& config)
    {
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getMusicFolders"), *config.parameters)};
        if (res.error)
        {
            return res.error.message;
        }

        const auto body(json::parse(res.text));
        const auto status = body.at("subsonic-response").at("status").get<std::string>();
        if (status == "ok")
            return body.dump(4);
        else 
            return body.at("subsonic-response").at("error").at("message").get<std::string>();
    }

    std::optional<std::string> getMusicDirectory(const server_config& config, const std::string& id)
    {
        auto parameters{*config.parameters};
        parameters.Add({"id", id});
        const auto res{cpr::Get(cpr::Url(config.url_string  + "/rest/getMusicDirectory"), parameters)};
        if (res.error)
        {
            return res.error.message;
        }

        const auto body(json::parse(res.text));
        const auto status = body.at("subsonic-response").at("status").get<std::string>();
        if (status == "ok")
            return body.dump(4);
        else 
            return body.at("subsonic-response").at("error").at("message").get<std::string>();

    }
}
