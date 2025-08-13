#pragma once
#include "oss/server_config.hpp"
import ftxui;

namespace ui
{
    class album_view
    {
    public:
        album_view(ftxui::ScreenInteractive& screen, oss::server_config& config);
        ftxui::Element render();
        inline ftxui::Component component() { return mContainer; }

    private:
        std::vector<std::string> get_albums();
        std::vector<std::vector<std::string>> get_albums_tracks();
        [[maybe_unused]] ftxui::ScreenInteractive* mScreen { nullptr };
        oss::server_config* mConfig { nullptr };

        int mAlbumSelected { 0 };
        std::vector<std::string> mAlbumTitles { get_albums() };
        std::vector<std::vector<std::string>> mAlbumTracks {};

        ftxui::Component mAlbumMenu { ftxui::Menu({ .entries = &mAlbumTitles, .selected = mAlbumSelected }) };
        ftxui::Component mTrackMenu { ftxui::Menu({ .entries = &mAlbumTitles, .selected = mAlbumSelected }) };
        ftxui::Component mContainer { ftxui::Container::Horizontal({ mAlbumMenu, mTrackMenu }) };
    };
} // namespace ui
