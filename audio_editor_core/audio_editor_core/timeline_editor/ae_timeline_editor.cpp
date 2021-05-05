//
//  ae_timeline_editor.cpp
//

#include "ae_timeline_editor.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_file_loader.h>
#include <cpp_utils/yas_fast_each.h>
#include <processing/yas_processing_umbrella.h>

using namespace yas;
using namespace yas::ae;

timeline_editor::timeline_editor(std::shared_ptr<timeline_file_loader_interface> const &file_loader,
                                 std::shared_ptr<timeline_player_interface> const &player)
    : _file_loader(file_loader), _player(player) {
}

void timeline_editor::setup(url const &url) {
    if (auto const file_info_opt = this->_file_loader->load_file_info(url)) {
        auto const &file_info = file_info_opt.value();

        auto const timeline = proc::timeline::make_shared();
        auto const track = proc::track::make_shared();
        auto const module = proc::file::make_signal_module<float>(url, 0, 0);

        auto each = make_fast_each(file_info.channel_count);
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            module->connect_output(idx, idx);
        }

        track->insert_module(module, 0, proc::time::range{0, file_info.length});
        timeline->insert_track(0, track);

        this->_player->set_timeline(timeline);
    } else {
        this->_player->reset_timeline();
    }
}

std::shared_ptr<timeline_editor> timeline_editor::make_shared(
    std::shared_ptr<timeline_player_interface> const &player) {
    return make_shared(app::global()->file_loader(), player);
}

std::shared_ptr<timeline_editor> timeline_editor::make_shared(
    std::shared_ptr<timeline_file_loader_interface> const &file_loader,
    std::shared_ptr<timeline_player_interface> const &player) {
    return std::shared_ptr<timeline_editor>(new timeline_editor{file_loader, player});
}
