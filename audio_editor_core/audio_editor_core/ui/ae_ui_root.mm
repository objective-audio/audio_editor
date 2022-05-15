//
//  ae_ui_root.mm
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_color.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_root_presenter.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_editing_root_level.h>
#include <audio_editor_core/ae_ui_editing_root_level_pool.h>

#include <cpp_utils/yas_assertion.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root> ui_root::make_shared(
    std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id,
    std::shared_ptr<ui_editing_root_level_pool> const &ui_editing_root_level_pool) {
    auto const &app_level = hierarchy::app_level();
    auto const presenter = root_presenter::make_shared(project_id.project_id);
    return std::shared_ptr<ui_root>(new ui_root{app_level->color, standard, presenter, ui_editing_root_level_pool});
}

ui_root::ui_root(std::shared_ptr<ae::color> const &color, std::shared_ptr<ui::standard> const &standard,
                 std::shared_ptr<root_presenter> const &presenter,
                 std::shared_ptr<ui_editing_root_level_pool> const &ui_editing_root_level_pool)
    : _presenter(presenter),
      _root_node(standard->root_node()),
      _color(color),
      _background(standard->view_look()->background()),
      _ui_editing_root_level_pool(ui_editing_root_level_pool) {
    standard->view_look()
        ->observe_appearance([this](auto const &) {
            if (auto const background = this->_background.lock()) {
                background->set_color(this->_color->background());
            }
        })
        .sync()
        ->add_to(this->_pool);
}

void ui_root::setup() {
    auto const &pool = this->_ui_editing_root_level_pool.lock();
    if (!pool || !pool->level()) {
        assertion_failure_if_not_test();
        return;
    }

    this->_root_node->add_sub_node(pool->level()->editing_root->node);
}

bool ui_root::responds_to_action(action const action) {
    auto const &pool = this->_ui_editing_root_level_pool.lock();
    if (!pool) {
        return false;
    }

    if (auto const &level = pool->level()) {
        return level->editing_root->responds_to_action(action);
    } else {
        return false;
    }
}
