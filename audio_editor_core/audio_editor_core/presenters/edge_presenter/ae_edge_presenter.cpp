//
//  ae_edge_presenter.cpp
//

#include "ae_edge_presenter.h"

#include <audio_editor_core/ae_app_level.h>
#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_file_loader_types.h>
#include <audio_editor_core/ae_project_pool.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<edge_presenter> edge_presenter::make_shared(std::string const &project_id,
                                                            std::shared_ptr<display_space> const &display_space) {
    auto const &editor = ae::app_level::global()->project_pool->project_for_id(project_id)->editor();
    return make_shared(editor, display_space);
}

std::shared_ptr<edge_presenter> edge_presenter::make_shared(
    std::shared_ptr<project_editor_for_edge_presenter> const &editor,
    std::shared_ptr<display_space> const &display_space) {
    return std::shared_ptr<edge_presenter>(new edge_presenter{editor, display_space});
}

edge_presenter::edge_presenter(std::shared_ptr<project_editor_for_edge_presenter> const &editor,
                               std::shared_ptr<display_space> const &display_space)
    : _locations(observing::value::holder<edge_locations>::make_shared({.begin = {.x = 0}, .end = {.x = 0}})),
      _editor(editor),
      _display_space(display_space) {
    editor->observe_edge_editor_event([this](edge_editor_event const &) { this->_update_locations(); })
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
    if (auto const editor = this->_editor.lock()) {
        auto const &edge = editor->edge();
        auto const sample_rate = editor->file_info().sample_rate;
        auto const &scale = this->_display_space->scale();

        this->_locations->set_value({.begin = edge_location::make_value(edge.begin_frame, sample_rate, scale),
                                     .end = edge_location::make_value(edge.end_frame, sample_rate, scale)});
    } else {
        this->_locations->set_value({.begin = {.x = 0}, .end = {.x = 0}});
    }
}
