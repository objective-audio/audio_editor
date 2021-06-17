//
//  ae_ui_root.h
//

#pragma once

#include <audio_editor_core/ae_project_dependency.h>
#include <audio_editor_core/ae_ui_button.h>
#include <audio_editor_core/ae_ui_root_dependency.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_root : project_ui_root_interface {
    std::shared_ptr<ui::renderer> const &renderer() const;

    static std::shared_ptr<ui_root> make_shared(std::shared_ptr<ui::renderer> const &,
                                                std::shared_ptr<project_view_presenter> const &);

   private:
    std::shared_ptr<project_view_presenter> const _presenter;

    std::shared_ptr<ui::renderer> const _renderer;
    std::shared_ptr<ui::font_atlas> const _font_atlas;
    std::shared_ptr<ui_button> const _play_button;
    std::shared_ptr<ui_button> const _split_button;
    std::shared_ptr<ui_button> const _drop_head_and_offset_button;
    std::shared_ptr<ui_button> const _drop_tail_and_offset_button;
    std::shared_ptr<ui_button> const _erase_and_offset_button;
    std::shared_ptr<ui_button> const _zero_button;
    std::vector<std::shared_ptr<ui_button>> const _buttons;
    std::shared_ptr<ui::collection_layout> const _button_collection_layout;
    std::shared_ptr<ui::strings> const _status_strings;
    std::shared_ptr<ui::strings> const _file_info_strings;
    std::shared_ptr<ui::strings> const _player_strings;
    std::shared_ptr<ui::strings> const _file_track_strings;

    observing::canceller_pool _pool;

    ui_root(std::shared_ptr<ui::renderer> const &, std::shared_ptr<project_view_presenter> const &);

    void _setup_node_hierarchie();
    void _setup_observing();
    void _setup_layout();
};
}  // namespace yas::ae
