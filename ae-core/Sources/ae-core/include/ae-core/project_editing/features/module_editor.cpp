//
//  module_editor.cpp
//

#include "module_editor.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project/features/player.h>
#include <ae-core/project_editing/features/editing_status.h>
#include <ae-core/project_editing/features/module_pool.h>
#include <ae-core/project_editing/features/pasteboard.h>

#include <ae-core/project/features/vertical_scrolling.hpp>
#include <ae-core/project_editing/features/selector_enabler.hpp>

using namespace yas;
using namespace yas::ae;

module_editor::module_editor(player *player, module_pool *module_pool, selected_module_pool *selected_module_pool,
                             selected_track_pool *selected_track_pool, vertical_scrolling const *vertical_scrolling,
                             pasteboard *pasteboard, editing_status const *editing_status,
                             selector_enabler const *selector_enabler)
    : _player(player),
      _module_pool(module_pool),
      _selected_module_pool(selected_module_pool),
      _selected_track_pool(selected_track_pool),
      _vertical_scrolling(vertical_scrolling),
      _pasteboard(pasteboard),
      _editing_status(editing_status),
      _selector_enabler(selector_enabler) {
}

bool module_editor::can_split() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_splittable_modules();
}

void module_editor::split() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();

    switch (this->_target_kind()) {
        case target_kind::modules: {
            auto const selected_modules = this->_selected_module_pool->elements();
            this->_selected_module_pool->clear();
            this->_module_pool->split(selected_modules, current_frame);
        } break;
        case target_kind::markers:
            // マーカー選択中はモジュールを分割対象としない
            return;
        case target_kind::tracks: {
            this->_module_pool->split_at(this->_selected_track_pool->elements(), current_frame);
        } break;
    }
}

void module_editor::drop_head() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();

    switch (this->_target_kind()) {
        case target_kind::modules: {
            auto const selected_modules = this->_selected_module_pool->elements();
            this->_selected_module_pool->clear();
            this->_module_pool->drop_head(selected_modules, current_frame);
        } break;
        case target_kind::markers:
            // マーカー選択中はモジュールを分割対象としない
            return;
        case target_kind::tracks: {
            this->_module_pool->drop_head_at(this->_selected_track_pool->elements(), current_frame);
        } break;
    }
}

void module_editor::drop_tail() {
    if (!this->can_split()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();

    switch (this->_target_kind()) {
        case target_kind::modules: {
            auto const selected_modules = this->_selected_module_pool->elements();
            this->_selected_module_pool->clear();
            this->_module_pool->drop_tail(selected_modules, current_frame);
        } break;
        case target_kind::markers:
            // マーカー選択中はモジュールを分割対象としない
            return;
        case target_kind::tracks: {
            this->_module_pool->drop_tail_at(this->_selected_track_pool->elements(), current_frame);
        } break;
    }
}

bool module_editor::can_erase() const {
    return this->can_copy();
}

void module_editor::erase() {
    if (!this->can_erase()) {
        return;
    }

    auto selected_modules = this->_selected_module_pool->elements();
    this->_erase_modules(this->_target_kind(), std::move(selected_modules));
}

bool module_editor::can_cut() const {
    return this->can_copy();
}

void module_editor::cut() {
    if (!this->can_cut()) {
        return;
    }

    // copyでクリアされるので先に保持しておく
    auto const target_kind = this->_target_kind();
    auto selected_modules = this->_selected_module_pool->elements();
    this->copy();
    this->_erase_modules(target_kind, std::move(selected_modules));
}

bool module_editor::can_copy() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    return this->_has_target_modules();
}

void module_editor::copy() {
    if (!this->can_copy()) {
        return;
    }

    auto const &module_pool = this->_module_pool;
    auto const current_frame = this->_player->current_frame();
    track_index_t const current_track = this->_vertical_scrolling->track();

    switch (this->_target_kind()) {
        case target_kind::modules: {
            auto const selected_modules = this->_selected_module_pool->elements();

            this->_selected_module_pool->clear();

            std::vector<pasting_module_object> pasting_modules;

            for (auto const &index : selected_modules) {
                if (auto const module = module_pool->module_at(index)) {
                    auto const &value = module.value().value;
                    pasting_modules.emplace_back(
                        pasting_module_object{identifier{},
                                              {value.name, value.file_frame, value.range.offset(-current_frame),
                                               value.track - current_track, value.file_name}});
                }
            }

            this->_pasteboard->set_modules(pasting_modules);
        } break;
        case target_kind::markers:
            // マーカー選択中はモジュールをコピー対象にしない
            break;
        case target_kind::tracks: {
            auto const modules = module_pool->modules_at(this->_selected_track_pool->elements(), current_frame);

            std::vector<pasting_module_object> pasting_modules;

            for (auto const &pair : modules) {
                auto const &value = pair.second.value;
                pasting_modules.emplace_back(
                    pasting_module_object{identifier{},
                                          {value.name, value.file_frame, value.range.offset(-current_frame),
                                           value.track - current_track, value.file_name}});
            }

            this->_pasteboard->set_modules(pasting_modules);
        } break;
    }
}

bool module_editor::can_paste() const {
    if (!this->_editing_status->can_editing()) {
        return false;
    }

    if (this->_pasteboard->modules().empty()) {
        return false;
    }

    return true;
}

void module_editor::paste() {
    if (!this->can_paste()) {
        return;
    }

    this->_selected_module_pool->clear();

    auto const &modules = this->_pasteboard->modules();
    if (modules.empty()) {
        return;
    }

    auto const current_frame = this->_player->current_frame();
    track_index_t const current_track = this->_vertical_scrolling->track();

    for (auto const &module : modules) {
        auto const &module_value = module.value;
        this->_module_pool->overwrite_module({module_value.name, module_value.range.offset(current_frame),
                                              module_value.track + current_track, module_value.file_frame,
                                              module_value.file_name});
    }
}

/// 編集対象の種類。無選択ならtrackにフォールバックする
module_editor::target_kind module_editor::_target_kind() const {
    if (auto const kind = this->_selector_enabler->current_kind(); kind.has_value()) {
        switch (kind.value()) {
            case selector_kind::module:
                return target_kind::modules;
            case selector_kind::marker:
                return target_kind::markers;
            case selector_kind::track:
                return target_kind::tracks;
        }
    } else {
        return target_kind::tracks;
    }
}

/// 編集対象となるモジュールが存在するかを返す
bool module_editor::_has_target_modules() const {
    switch (this->_target_kind()) {
        case target_kind::modules:
            // 選択されたモジュールが編集対象となる
            return true;
        case target_kind::markers:
            // マーカー選択中はモジュールを編集対象としない
            return false;
        case target_kind::tracks: {
            // トラックが編集対象なら、選択中または全てのトラックの再生位置にモジュールが存在すればtrue
            auto const current_frame = this->_player->current_frame();
            return this->_module_pool->has_modules_at(this->_selected_track_pool->elements(), current_frame);
        }
    }
}

bool module_editor::_has_splittable_modules() const {
    auto const current_frame = this->_player->current_frame();

    switch (this->_target_kind()) {
        case target_kind::modules:
            // 選択されたモジュールが分割対象となる
            return this->_module_pool->has_splittable_modules(this->_selected_module_pool->elements(), current_frame);
        case target_kind::markers:
            // マーカー選択中はモジュールを分割対象としない
            return false;
        case target_kind::tracks: {
            return this->_module_pool->has_splittable_modules_at(this->_selected_track_pool->elements(), current_frame);
        }
    }
}

/// cutするときにcopyした時点でクリアされてしまっているのでtarget_kindやselected_modulesを引数で渡す
void module_editor::_erase_modules(target_kind const target_kind, selected_module_set &&selected_modules) {
    switch (target_kind) {
        case target_kind::modules: {
            this->_selected_module_pool->clear();

            for (auto const &index : selected_modules) {
                this->_module_pool->erase_module_and_notify(index);
            }
        } break;
        case target_kind::markers:
            // マーカー選択中はモジュールを削除対象にしない
            return;
        case target_kind::tracks: {
            auto const current_frame = this->_player->current_frame();
            this->_module_pool->erase_at(this->_selected_track_pool->elements(), current_frame);
        } break;
    }
}
