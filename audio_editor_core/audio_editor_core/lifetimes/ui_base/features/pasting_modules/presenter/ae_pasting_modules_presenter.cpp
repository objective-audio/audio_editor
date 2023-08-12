//
//  ae_pasting_modules_presenter.cpp
//

#include "ae_pasting_modules_presenter.hpp"

#include <audio_editor_core/ae_display_space.h>
#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_pasteboard.h>
#include <cpp_utils/yas_lock.h>

#include <audio_editor_core/ae_display_space_range.hpp>
#include <audio_editor_core/ae_vertical_scrolling.hpp>

using namespace yas;
using namespace yas::ae;

static std::vector<std::optional<pasting_module_content>> const _empty_contents;

std::shared_ptr<pasting_modules_presenter> pasting_modules_presenter::make_shared(
    project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<pasting_modules_presenter>(
        project_editing_lifetime->project_format, project_editing_lifetime->pasteboard, project_lifetime->display_space,
        project_editing_lifetime->display_space_range, project_editing_lifetime->pasting_module_content_pool,
        project_lifetime->vertical_scrolling);
}

pasting_modules_presenter::pasting_modules_presenter(project_format const &project_format,
                                                     std::shared_ptr<pasteboard> const &pasteboard,
                                                     std::shared_ptr<display_space> const &display_space,
                                                     std::shared_ptr<display_space_range> const &display_space_range,
                                                     std::shared_ptr<pasting_module_content_pool> const &content_pool,
                                                     std::shared_ptr<vertical_scrolling> const &scrolling)
    : _project_format(project_format),
      _pasteboard(pasteboard),
      _display_space(display_space),
      _display_space_range(display_space_range),
      _content_pool(content_pool),
      _scrolling(scrolling) {
    pasteboard
        ->observe_event([this](pasteboard_event const &event) {
            switch (event) {
                case pasteboard_event::fetched:
                case pasteboard_event::modules_replaced:
                case pasteboard_event::modules_cleared:
                    this->_update_all_contents(true, true);
                    break;
                case pasteboard_event::markers_replaced:
                case pasteboard_event::markers_cleared:
                    break;
            }
        })
        .sync()
        ->add_to(this->_canceller_pool);

    // pasteboardのfetchedでupdateしているのでendにして最初はスルー
    display_space->observe([this](display_space_event const &event) { this->_update_all_contents(true, false); })
        .end()
        ->add_to(this->_canceller_pool);
}

std::vector<std::optional<pasting_module_content>> const &pasting_modules_presenter::contents() const {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->elements();
    } else {
        return _empty_contents;
    }
}

observing::syncable pasting_modules_presenter::observe_contents(
    std::function<void(pasting_module_content_pool_event const &)> &&handler) {
    if (auto const content_pool = this->_content_pool.lock()) {
        return content_pool->observe_event(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

double pasting_modules_presenter::y_offset() const {
    if (auto const scrolling = this->_scrolling.lock()) {
        return -scrolling->position_offset();
    } else {
        return 0;
    }
}

void pasting_modules_presenter::update_if_needed() {
    this->_update_all_contents(false, false);
}

std::optional<ae::space_range> pasting_modules_presenter::_space_range() const {
    if (auto const display_space_range = this->_display_space_range.lock()) {
        auto const space_range = display_space_range->zero();
        if (space_range.has_value()) {
            return space_range.value();
        }
    }
    return std::nullopt;
}

void pasting_modules_presenter::_update_all_contents(bool const force_updating, bool const force_replacing) {
    auto const space_range = this->_space_range();

    if (!space_range.has_value()) {
        return;
    }

    if (space_range == this->_last_space_range && !force_updating) {
        return;
    }

    auto const locked = yas::lock(this->_pasteboard, this->_display_space, this->_content_pool);

    if (!fulfilled(locked)) {
        return;
    }

    auto const &[pasteboard, display_space, content_pool] = locked;
    auto const scale = display_space->scale().width;

    auto const contents = filter_map<pasting_module_content>(
        pasteboard->modules(),
        [&space_range, sample_rate = this->_project_format.sample_rate, &scale](pasting_module_object const &module) {
            if (module.index().is_overlap(space_range.value())) {
                return std::make_optional<pasting_module_content>(module, sample_rate, scale);
            } else {
                return std::optional<pasting_module_content>(std::nullopt);
            }
        });

    content_pool->update_all(contents, force_replacing);

    this->_last_space_range = space_range;
}
