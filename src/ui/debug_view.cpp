#include "ui/debug_view.hpp"
#include <format>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
import ftxui;

namespace ui
{
    debug_view::debug_view(ftxui::ScreenInteractive& screen, oss::server_config& config)
        : mScreen{&screen}
        , mConfig{&config}
        , mContainer(ftxui::Container::Vertical(
            {
                mQuit
            } , &mIndex
        ))
    {

    }

    ftxui::Element debug_view::render()
    {
        return ftxui::vbox({
            ftxui::text("Debugging Page"),
            ftxui::separator(),
            ftxui::text(std::format("Connected to server: {}", mConfig->url_string)),
            ftxui::text(std::format("User: {}", mConfig->user)),
            ftxui::separator(),
            mQuit->Render()
        }) | ftxui::border;
    }
}
