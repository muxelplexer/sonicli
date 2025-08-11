#pragma once
#include "oss/server_config.hpp"
import ftxui;

namespace ui
{
    class debug_view final
    {
    public:
        debug_view(ftxui::ScreenInteractive& screen, oss::server_config& config);
        ftxui::Element render();
        inline ftxui::Component component()
        {
            return mContainer;
        }

    private:
        void ping();

        int mIndex{0};
        oss::server_config* mConfig{nullptr};
        ftxui::ScreenInteractive* mScreen{nullptr};
        ftxui::Component mQuit{ftxui::Button("Quit", [&]{ mScreen->Exit(); })};
        ftxui::Component mPing{ftxui::Button("Ping!", [&]{ mScreen->Exit(); })};
        ftxui::Component mContainer;
    };
}
