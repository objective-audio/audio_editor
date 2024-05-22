//
//  ae_module_selector.cpp
//

#include "ae_module_selector.hpp"

#include <ae-core/project_editing/features/ae_editing_status.h>
#include <ae-core/project_editing/features/ae_module_pool.h>
#include <cpp-utils/assertion.h>
#include <cpp-utils/stl_utils.h>

#include <ae-core/project_editing/features/ae_selector_enabler.hpp>

using namespace yas;
using namespace yas::ae;

module_selector::module_selector(module_pool const *module_pool, selected_module_pool *selected_pool,
                                 selected_track_pool *selected_track_pool, editing_status const *editing_status,
                                 selector_enabler const *enabler)
    : _module_pool(module_pool),
      _selected_module_pool(selected_pool),
      _selected_track_pool(selected_track_pool),
      _editing_status(editing_status),
      _enabler(enabler) {
}

bool module_selector::can_select() const {
    return this->_editing_status->can_editing();
}

void module_selector::begin_selection() {
    this->_selected_module_pool->begin_toggling();
}

void module_selector::select(std::set<module_index> const &indices) {
    if (this->_enabler->is_enabled(selector_kind::module)) {
        this->_selected_module_pool->toggle(indices);
    }
}

void module_selector::end_selection() {
    this->_selected_module_pool->end_toggling();
}

bool module_selector::can_toggle() const {
    return this->_editing_status->can_editing();
}

void module_selector::toggle(module_index const &index) {
    if (this->_module_pool->modules().contains(index)) {
        this->begin_selection();
        this->select({index});
        this->end_selection();
    } else {
        assertion_failure_if_not_test();
    }
}

void module_selector::select_all() {
    auto const &selected_tracks = this->_selected_track_pool->elements();

    if (this->_enabler->is_enabled(selector_kind::module) && selected_tracks.empty()) {
        auto modules =
            yas::to_set<module_index>(this->_module_pool->modules(), [](auto const &pair) { return pair.first; });
        this->_selected_module_pool->replace(std::move(modules));
    } else if (this->_enabler->is_enabled(selector_kind::track) && !selected_tracks.empty()) {
        std::set<module_index> modules;

        for (auto const &pair : this->_module_pool->modules()) {
            if (selected_tracks.contains(pair.first.track)) {
                modules.emplace(pair.first);
            }
        }

        this->_selected_module_pool->replace(std::move(modules));
    }
}
