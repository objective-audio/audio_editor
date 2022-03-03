//
//  ae_ui_root.mm
//

#include "ae_ui_root.h"
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_pinch_gesture_controller.h>
#include <audio_editor_core/ae_root_presenter.h>
#include <audio_editor_core/ae_ui_editing_root.h>
#include <audio_editor_core/ae_ui_editing_root_level.h>
#include <audio_editor_core/ae_ui_editing_root_level_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_root> ui_root::make_shared(
    std::shared_ptr<ui::standard> const &standard, ui_project_id const &project_id,
    std::shared_ptr<ui_editing_root_level_pool> const &ui_editing_root_level_pool) {
    auto const presenter = root_presenter::make_shared(project_id.identifier);
    return std::shared_ptr<ui_root>(new ui_root{standard->root_node(), presenter, ui_editing_root_level_pool});
}

ui_root::ui_root(std::shared_ptr<ui::node> const &root_node, std::shared_ptr<root_presenter> const &presenter,
                 std::shared_ptr<ui_editing_root_level_pool> const &ui_editing_root_level_pool)
    : _presenter(presenter), _root_node(root_node), _ui_editing_root_level_pool(ui_editing_root_level_pool) {
    presenter
        ->observe_is_editing([this](bool const &is_editing) {
            auto const &pool = this->_ui_editing_root_level_pool.lock();
            if (!pool) {
                return;
            }

            if (is_editing) {
                if (pool->level() == nullptr) {
                    pool->add_level();

                    auto const &level = pool->level();
                    this->_root_node->add_sub_node(level->editing_root->node);
                }
            } else {
                if (auto const &level = pool->level()) {
                    level->editing_root->node->remove_from_super_node();
                    pool->remove_level();
                }
            }
        })
        .sync()
        ->add_to(this->_pool);
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
