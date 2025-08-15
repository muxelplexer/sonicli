#include "oss/server_config.hpp"
#include "ui/album_view.hpp"
#include "ui/login_component.hpp"

import nlohmann.json;
import ftxui;

int main()
{
    auto config { oss::server_config::from_file().value_or({"", ""}) };
    auto screen { ftxui::ScreenInteractive::Fullscreen() };

    if (!config.password)
    {
        ui::login_component login { screen, config };
        auto login_renderer = ftxui::Renderer(login.component(), [&] { return login.render(); });

        screen.Loop(login_renderer);
    }

    ui::album_view album { screen, config };
    screen.Loop(album.render());

    return 0;
}
