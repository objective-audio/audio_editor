//
//  ae_marker_name_presenter.cpp
//

#include "ae_marker_name_presenter.hpp"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>

#include <audio_editor_core/ae_marker_name_editor.hpp>
#include <audio_editor_core/ae_marker_name_sheet_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<marker_name_presenter> marker_name_presenter::make_shared(project_sub_lifetime_id const &lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(lifetime_id.window);
    auto const &sheet_lifetime = hierarchy::marker_name_sheet_lifetime_for_id(lifetime_id);

    return std::make_shared<marker_name_presenter>(lifetime_id, sheet_lifetime->editor,
                                                   project_lifetime->modal_lifecycle);
}

marker_name_presenter::marker_name_presenter(project_sub_lifetime_id const &lifetime_id,
                                             std::shared_ptr<marker_name_editor> const &editor,
                                             std::shared_ptr<project_modal_lifecycle> const &lifecycle)
    : _lifetime_id(lifetime_id), _editor(editor), _lifecycle(lifecycle) {
}

std::string marker_name_presenter::name() const {
    if (auto const editor = this->_editor.lock()) {
        return editor->name();
    }
    return "";
}

void marker_name_presenter::done(std::string const &name) {
    if (auto const editor = this->_editor.lock()) {
        editor->set_name(name);
    }

    this->_finalize();
}

void marker_name_presenter::cancel() {
    this->_finalize();
}

void marker_name_presenter::_finalize() {
    if (auto const lifecycle = this->_lifecycle.lock()) {
        lifecycle->remove_marker_name_sheet(this->_lifetime_id);
    }

    this->_editor.reset();
    this->_lifecycle.reset();
}
