#pragma once
#include <ftxui/component/component.hpp>
#include <string>
#include <vector>

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/node.hpp>

#include "oss/server_config.hpp"

namespace ui
{
    class login_component final
    {
    public:
        login_component(ftxui::ScreenInteractive& screen, oss::server_config& config);
        ftxui::Element render();
        ftxui::Component component() { return mContainer; }

    private:
        ftxui::ScreenInteractive* mScreen { nullptr };
        std::vector<std::string> entries { "login" };
        int index { 0 };
        int buttonIndex { 0 };
        oss::server_config* mConfig { nullptr };
        bool mConnection { false };
        std::string serverText;
        std::string mPassword;

        ftxui::Component mUserNameInput { ftxui::Input(&mConfig->user, "username") };
        ftxui::Component mPasswordInput { ftxui::Input(&mPassword, "", { .password = true }) };
        ftxui::Component mURLInput { ftxui::Input(&mConfig->url_string, "https://") };
        ftxui::Component mQuit { ftxui::Button("Quit", [&] { mScreen->Exit(); }) };
        ftxui::Component mSubmit;

        ftxui::Component mContainer;
    };
} // namespace ui
