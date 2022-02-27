//
//  ae_edge_presenter.cpp
//

#include "ae_edge_presenter.h"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_edge_editor.h>
#include <audio_editor_core/ae_file_info.h>
#include <audio_editor_core/ae_hierarchy.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_presenter> edge_presenter::make_shared(std::string const &project_id,
                                                            std::shared_ptr<display_space> const &display_space) {
    auto const &editor_level = hierarchy::project_editor_level_for_id(project_id);
    return std::shared_ptr<edge_presenter>(
        new edge_presenter{editor_level->file_info, editor_level->edge_editor, display_space});
}

edge_presenter::edge_presenter(file_info const &file_info, std::shared_ptr<edge_editor> const &edge_editor,
                               std::shared_ptr<display_space> const &display_space)
    : _file_info(file_info),
      _locations(observing::value::holder<edge_locations>::make_shared({.begin = {.x = 0}, .end = {.x = 0}})),
      _edge_editor(edge_editor),
      _display_space(display_space) {
    edge_editor->observe_event([this](edge_editor_event const &) { this->_update_locations(); })
        .end()
        ->add_to(this->_pool);

    display_space->observe([this](display_space_event const &) { this->_update_locations(); })
        .sync()
        ->add_to(this->_pool);
}

edge_locations edge_presenter::locations() const {
    return this->_locations->value();
}

observing::syncable edge_presenter::observe_locations(std::function<void(edge_locations const &)> &&handler) {
    return this->_locations->observe(std::move(handler));
}

void edge_presenter::_update_locations() {
    if (auto const edge_editor = this->_edge_editor.lock()) {
        auto const &edge = edge_editor->edge();
        auto const sample_rate = this->_file_info.sample_rate;
        auto const &scale = this->_display_space->scale();

        this->_locations->set_value({.begin = edge_location::make_value(edge.begin_frame, sample_rate, scale),
                                     .end = edge_location::make_value(edge.end_frame, sample_rate, scale)});
    } else {
        this->_locations->set_value({.begin = {.x = 0}, .end = {.x = 0}});
    }
}
