//
//  module_name_presenter.cpp
//

#include "module_name_presenter.h"

#include <ae-core/global/hierarchy.h>
#include <ae-core/project_editing/lifecycles/project_modal_lifecycle.h>

#include <ae-core/module_name_sheet/features/module_name_editor.hpp>

using namespace yas;
using namespace yas::ae;

static std::string const empty_string = "";

std::shared_ptr<module_name_presenter> module_name_presenter::make_shared(project_sub_lifetime_id const &lifetime_id) {
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(lifetime_id.project);
    auto const &sheet_lifetime = hierarchy::module_name_sheet_lifetime_for_id(lifetime_id);
    return std::make_shared<module_name_presenter>(lifetime_id, sheet_lifetime->editor,
                                                   project_editing_lifetime->modal_lifecycle);
}

module_name_presenter::module_name_presenter(project_sub_lifetime_id const &lifetime_id,
                                             std::shared_ptr<module_name_editor> const &module_name_editor,
                                             std::shared_ptr<project_modal_lifecycle> const &lifecycle)
    : _lifetime_id(lifetime_id), _module_name_editor(module_name_editor), _lifecycle(lifecycle) {
}

std::string const &module_name_presenter::name() const {
    if (auto const editor = this->_module_name_editor.lock()) {
        return editor->name();
    }

    return empty_string;
}

void module_name_presenter::done(std::string const &name) {
    if (auto const editor = this->_module_name_editor.lock()) {
        editor->set_name(name);
    }

    this->_finalize();
}

void module_name_presenter::cancel() {
    this->_finalize();
}

void module_name_presenter::_finalize() {
    if (auto const lifecycle = this->_lifecycle.lock()) {
        lifecycle->remove_module_name_sheet(this->_lifetime_id);
    }

    this->_module_name_editor.reset();
    this->_lifecycle.reset();
}
