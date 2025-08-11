#include "ui/login_component.hpp"
#include "oss/server_config.hpp"

import nlohmann.json;
import ftxui;

int main()
{
    namespace ui = ftxui;

    oss::server_config config{"default_name", "https://localhost"};
    auto screen{ui::ScreenInteractive::Fullscreen()};

    login_component login{screen, config};
    auto renderer = ui::Renderer(login.component(), [&]{
        return login.render();
    });

    screen.Loop(renderer);
    return 0;
}
