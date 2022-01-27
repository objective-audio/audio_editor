//
//  ae_time_presenter.cpp
//

#include "ae_time_presenter.h"

#include <audio_editor_core/ae_app.h>
#include <audio_editor_core/ae_project.h>
#include <audio_editor_core/ae_project_editor.h>
#include <audio_editor_core/ae_project_pool.h>
#include <audio_editor_core/ae_time_presenter_utils.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_presenter> time_presenter::make_shared(std::string const project_id) {
    auto const project_editor = app::global()->project_pool()->project_for_id(project_id)->editor();
    return std::shared_ptr<time_presenter>(new time_presenter{project_editor, project_editor->timing()});
}

time_presenter::time_presenter(std::shared_ptr<project_editor_for_time_presenter> const &project_editor,
                               std::shared_ptr<timing_for_time_presenter> const &timing)
    : _project_editor(project_editor), _timing(timing) {
    this->_range_fetcher =
        observing::fetcher<std::optional<index_range>>::make_shared([this] { return this->editing_time_text_range(); });

    project_editor
        ->observe_time_editor_for_time_presenter(
            [this, cancellable = observing::cancellable_ptr{nullptr}](
                std::shared_ptr<time_editor_for_time_presenter> const &editor) mutable {
                this->_time_editor = editor;

                if (editor) {
                    cancellable =
                        editor->observe_unit_index([this](auto const &) { this->_range_fetcher->push(); }).sync();
                } else {
                    cancellable = nullptr;
                    this->_range_fetcher->push();
                }
            })
        .sync()
        ->add_to(this->_pool);
}

std::string time_presenter::time_text() const {
    if (auto const time_editor = this->_time_editor.lock()) {
        return time_presenter_utils::time_text(time_editor->editing_components());
    } else {
        auto const editor = this->_project_editor.lock();
        auto const timing = this->_timing.lock();
        if (editor && timing) {
            return time_presenter_utils::time_text(timing->components(editor->current_frame()).raw_components());
        } else {
            return "";
        }
    }
}

std::optional<index_range> time_presenter::editing_time_text_range() const {
    if (auto const time_editor = this->_time_editor.lock()) {
        return time_presenter_utils::to_time_text_range(time_editor->editing_components(), time_editor->unit_index());
    } else {
        return std::nullopt;
    }
}

observing::syncable time_presenter::observe_editing_time_text_range(
    std::function<void(std::optional<index_range> const &)> &&handler) {
    return this->_range_fetcher->observe(std::move(handler));
}
