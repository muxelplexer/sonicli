#include "oss/server_config.hpp"
#include "oss/endpoints.hpp"
#include <memory>

import nlohmann.json;

namespace oss
{
    server_config::server_config(const std::string& user, const std::string& url)
        : user{user},  url_string{url}
    {
    }

    bool server_config::login()
    {
        if (!parameters)
            parameters = std::make_unique<cpr::Parameters>();

        parameters->Add({"v","1.1"});
        parameters->Add({"f","json"});
        parameters->Add({"c",std::string(client_id)});
        parameters->Add({"u",user});
        if (!password)
            return false;

        parameters->Add({"s",password->salt()});
        parameters->Add({"t",password->hash()});

        if (oss::ping(*this).has_value())
        {
            parameters.reset();
            return false;
        } else
        {
            return true;
        }
    }
}

