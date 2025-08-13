#include "ui/album_view.hpp"
#include "ui/debug_view.hpp"
#include "ui/login_component.hpp"
#include "oss/server_config.hpp"

import nlohmann.json;
import ftxui;

int main()
{
    oss::server_config config{"", ""};
    auto screen{ftxui::ScreenInteractive::Fullscreen()};

    ui::login_component login{screen, config};
    auto login_renderer = ftxui::Renderer(login.component(), [&]{
        return login.render();
    });

    screen.Loop(login_renderer);

    if (!config.password)
        return 1;

    ui::album_view album{screen, config};
    auto album_renderer = ftxui::Renderer(album.component(), [&]{
        return album.render();
    });
    screen.Loop(album_renderer);

    ui::debug_view debug{screen, config};
    auto debug_renderer = ftxui::Renderer(debug.component(), [&]{
        return debug.render();
    });
    screen.Loop(debug_renderer);

    return 0;
}
