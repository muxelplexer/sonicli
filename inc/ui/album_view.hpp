#pragma once
#include "oss/data/subsonic_response.hpp"
#include "oss/server_config.hpp"
#include <ftxui/component/component_options.hpp>
#include <memory>
#include <thread>
#include <unordered_map>
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
        std::vector<std::string> get_current_albums_tracks();

        void stream_start();
        void stream_stop();
        std::unique_ptr<std::thread> mStreamThread { nullptr };

        [[maybe_unused]] ftxui::ScreenInteractive* mScreen { nullptr };
        oss::server_config* mConfig { nullptr };

        int mAlbumSelected { 0 };
        int mTrackSelected { 0 };
        std::vector<oss::data::music_track> mAlbums;
        std::vector<std::string> mAlbumTitles { get_albums() };
        std::unordered_map<std::string, std::vector<oss::data::music_track>> mAlbumTracks;
        std::vector<std::string> mCurrentAlbumTracks { get_current_albums_tracks() };

        ftxui::MenuOption albumOption { .entries   = &mAlbumTitles,
                                        .selected  = &mAlbumSelected,
                                        .on_change = [&]()
                                        {
                                            mTrackSelected      = 0;
                                            mCurrentAlbumTracks = get_current_albums_tracks();
                                        } };
        ftxui::Component mAlbumMenu { ftxui::Menu(albumOption) };
        ftxui::Component mTrackMenu { ftxui::Menu({ .entries = &mCurrentAlbumTracks, .selected = &mTrackSelected }) };
        ftxui::Component mContainer { ftxui::Container::Horizontal({ mAlbumMenu, mTrackMenu }) };
    };
} // namespace ui
