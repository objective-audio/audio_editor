//
//  ae_time_numbers_presenter.cpp
//

#include "ae_time_numbers_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_modal_lifecycle.h>
#include <audio_editor_core/ae_time_editing_lifetime.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_numbers_presenter_utils.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_fast_each.h>
#include <cpp_utils/yas_lock.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_numbers_presenter> time_numbers_presenter::make_shared(
    project_lifetime_id const &project_lifetime_id) {
    auto const &project_lifetime = hierarchy::project_lifetime_for_id(project_lifetime_id);
    auto const &project_editing_lifetime = hierarchy::project_editing_lifetime_for_id(project_lifetime_id);
    return std::make_shared<time_numbers_presenter>(project_editing_lifetime->timing, project_lifetime->player,
                                                    project_editing_lifetime->modal_lifecycle);
}

time_numbers_presenter::time_numbers_presenter(std::shared_ptr<timing> const &timing,
                                               std::shared_ptr<player> const &player,
                                               std::shared_ptr<project_modal_lifecycle> const &project_modal_lifecycle)
    : _timing(timing), _player(player), _project_modal_lifecycle(project_modal_lifecycle) {
    this->_range_fetcher =
        observing::fetcher<std::optional<index_range>>::make_shared([this] { return this->editing_time_text_range(); });

    project_modal_lifecycle
        ->observe([this, cancellable = observing::cancellable_ptr{nullptr}](
                      std::optional<project_modal_sub_lifetime> const &sub_lifetime) mutable {
            if (auto const &lifetime = get<time_editing_lifetime>(sub_lifetime)) {
                cancellable =
                    lifetime->editor->observe_unit_index([this](auto const &) { this->_range_fetcher->push(); }).sync();
            } else {
                cancellable = nullptr;
                this->_range_fetcher->push();
            }
        })
        .sync()
        ->add_to(this->_pool);
}

std::string time_numbers_presenter::time_text() const {
    if (auto const &lifetime = this->_lifetime()) {
        return time_numbers_presenter_utils::time_text(lifetime->editor->editing_components());
    } else {
        if (auto const locked = yas::lock(this->_player, this->_timing); fulfilled(locked)) {
            auto const &[player, timing] = locked;

            return time_numbers_presenter_utils::time_text(
                timing->components(player->current_frame()).raw_components());
        } else {
            return "";
        }
    }
}

std::vector<index_range> time_numbers_presenter::time_text_unit_ranges() const {
    if (auto const timing = this->_timing.lock()) {
        auto const components = timing->components(0).raw_components();

        std::vector<index_range> ranges;
        ranges.reserve(components.size());

        auto each = make_fast_each(components.size());
        while (yas_each_next(each)) {
            auto const &idx = yas_each_index(each);
            ranges.emplace_back(time_numbers_presenter_utils::to_time_text_range(components, idx));
        }

        return ranges;
    } else {
        return {};
    }
}

std::optional<std::size_t> time_numbers_presenter::editing_unit_index() const {
    if (auto const &lifetime = this->_lifetime()) {
        return lifetime->editor->unit_index();
    } else {
        return std::nullopt;
    }
}

std::optional<index_range> time_numbers_presenter::editing_time_text_range() const {
    if (auto &lifetime = this->_lifetime()) {
        auto const &time_editor = lifetime->editor;
        return time_numbers_presenter_utils::to_time_text_range(time_editor->editing_components(),
                                                                time_editor->unit_index());
    } else {
        return std::nullopt;
    }
}

observing::syncable time_numbers_presenter::observe_editing_time_text_range(
    std::function<void(std::optional<index_range> const &)> &&handler) {
    return this->_range_fetcher->observe(std::move(handler));
}

std::shared_ptr<time_editing_lifetime> const &time_numbers_presenter::_lifetime() const {
    if (auto const lifecycle = this->_project_modal_lifecycle.lock()) {
        return lifecycle->time_editing_lifetime();
    }

    static std::shared_ptr<time_editing_lifetime> const null_lifetime;
    return null_lifetime;
}
