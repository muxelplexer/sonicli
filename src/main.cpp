#include "oss/server_config.hpp"
#include "ui/album_view.hpp"
#include "ui/login_component.hpp"

import nlohmann.json;
import ftxui;

int main()
{
    oss::server_config config { "", "" };
    auto screen { ftxui::ScreenInteractive::Fullscreen() };

    ui::login_component login { screen, config };
    auto login_renderer = ftxui::Renderer(login.component(), [&] { return login.render(); });

    screen.Loop(login_renderer);

    if (!config.password)
    {
        return 1;
    }

    ui::album_view album { screen, config };
    screen.Loop(album.render());

    return 0;
}
