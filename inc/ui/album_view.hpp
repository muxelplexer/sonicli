#pragma once
#include "oss/server_config.hpp"
#include <ftxui/component/component_options.hpp>
import ftxui;

namespace ui
{
    class album_view
    {
    public:
        album_view(ftxui::ScreenInteractive& screen, oss::server_config& config);
        ftxui::Component render();

    private:
        std::vector<std::string> get_albums();
        std::vector<std::vector<std::string>> get_albums_tracks();
        [[maybe_unused]] ftxui::ScreenInteractive* mScreen { nullptr };
        oss::server_config* mConfig { nullptr };

        int mAlbumSelected { 0 };
        int mTrackSelected { 0 };
        std::vector<std::string> mAlbumTitles { get_albums() };
        std::vector<std::vector<std::string>> mAlbumTracks { get_albums_tracks() };
        std::vector<std::string> mCurrentAlbumTracks;

        ftxui::MenuOption albumOption { .entries   = &mAlbumTitles,
                                        .selected  = &mAlbumSelected,
                                        .on_change = [&]()
                                        {
                                            mTrackSelected      = 0;
                                            mCurrentAlbumTracks = mAlbumTracks[mAlbumSelected];
                                        } };
        ftxui::Component mAlbumMenu { ftxui::Menu(albumOption) };
        ftxui::Component mTrackMenu { ftxui::Menu({ .entries = &mCurrentAlbumTracks, .selected = &mTrackSelected }) };
        ftxui::Component mContainer { ftxui::Container::Horizontal({ mAlbumMenu, mTrackMenu }) };
    };
} // namespace ui
