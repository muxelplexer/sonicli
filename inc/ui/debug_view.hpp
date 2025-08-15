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
        inline ftxui::Component component() { return mContainer; }

    private:
        void ping();
        void musicFolders();
        void albumList();
        std::string mDebugText { "" };

        int mIndex { 0 };
        oss::server_config* mConfig { nullptr };
        ftxui::ScreenInteractive* mScreen { nullptr };
        ftxui::Component mQuit { ftxui::Button("Quit", [&] { mScreen->Exit(); }) };
        ftxui::Component mPing { ftxui::Button("Ping!", [&] { this->ping(); }) };
        ftxui::Component mMusicFolders { ftxui::Button("MusicFolders", [&] { this->musicFolders(); }) };
        ftxui::Component mAlbumList { ftxui::Button("AlbumList", [&] { this->albumList(); }) };
        ftxui::Component mContainer;
    };
} // namespace ui
