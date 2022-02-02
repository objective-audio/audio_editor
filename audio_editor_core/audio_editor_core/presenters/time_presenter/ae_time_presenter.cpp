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

std::vector<index_range> time_presenter::time_text_unit_ranges() const {
    if (auto const timing = this->_timing.lock()) {
        auto const components = timing->components(0).raw_components();

        std::vector<index_range> ranges;
        ranges.reserve(components.size());

        auto each = make_fast_each(components.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            ranges.emplace_back(time_presenter_utils::to_time_text_range(components, idx));
        }

        return ranges;
    } else {
        return {};
    }
}

std::optional<std::size_t> time_presenter::editing_unit_index() const {
    if (auto const time_editor = this->_time_editor.lock()) {
        return time_editor->unit_index();
    } else {
        return std::nullopt;
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

std::optional<std::size_t> time_presenter::nudging_unit_index() const {
    if (auto const editor = this->_project_editor.lock()) {
        return time_presenter_utils::to_unit_index(editor->nudging_kind());
    } else {
        return std::nullopt;
    }
}

observing::syncable time_presenter::observe_nudging_unit_index(std::function<void(std::size_t const &)> &&handler) {
    if (auto const editor = this->_project_editor.lock()) {
        return editor->observe_nudging_kind([handler = std::move(handler)](ae::nudging_kind const &kind) {
            handler(time_presenter_utils::to_unit_index(kind));
        });
    } else {
        return observing::syncable{};
    }
}
