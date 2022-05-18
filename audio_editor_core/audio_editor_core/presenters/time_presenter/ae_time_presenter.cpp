//
//  ae_time_presenter.cpp
//

#include "ae_time_presenter.h"

#include <audio_editor_core/ae_hierarchy.h>
#include <audio_editor_core/ae_nudge_settings.h>
#include <audio_editor_core/ae_player.h>
#include <audio_editor_core/ae_project_sub_level_router.h>
#include <audio_editor_core/ae_time_editor.h>
#include <audio_editor_core/ae_time_editor_level.h>
#include <audio_editor_core/ae_time_presenter_utils.h>
#include <audio_editor_core/ae_timing.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<time_presenter> time_presenter::make_shared(project_id const &project_id) {
    auto const &project_level = hierarchy::project_level_for_id(project_id);
    return std::shared_ptr<time_presenter>(new time_presenter{
        project_level->timing, project_level->player, project_level->nudge_settings, project_level->sub_level_router});
}

time_presenter::time_presenter(std::shared_ptr<timing> const &timing, std::shared_ptr<player> const &player,
                               std::shared_ptr<nudge_settings> const &nudge_settings,
                               std::shared_ptr<project_sub_level_router> const &project_sub_level_router)
    : _timing(timing),
      _player(player),
      _nudge_settings(nudge_settings),
      _project_sub_level_router(project_sub_level_router) {
    this->_range_fetcher =
        observing::fetcher<std::optional<index_range>>::make_shared([this] { return this->editing_time_text_range(); });

    project_sub_level_router
        ->observe([this, cancellable = observing::cancellable_ptr{nullptr}](auto const &) mutable {
            auto const &router = this->_project_sub_level_router.lock();
            if (!router) {
                return;
            }

            if (auto const &level = router->time_editor_level()) {
                cancellable =
                    level->editor->observe_unit_index([this](auto const &) { this->_range_fetcher->push(); }).sync();
            } else {
                cancellable = nullptr;
                this->_range_fetcher->push();
            }
        })
        .sync()
        ->add_to(this->_pool);
}

std::string time_presenter::time_text() const {
    if (auto const &level = this->_level()) {
        return time_presenter_utils::time_text(level->editor->editing_components());
    } else {
        auto const player = this->_player.lock();
        auto const timing = this->_timing.lock();
        if (player && timing) {
            return time_presenter_utils::time_text(timing->components(player->current_frame()).raw_components());
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
    if (auto const &level = this->_level()) {
        return level->editor->unit_index();
    } else {
        return std::nullopt;
    }
}

std::optional<index_range> time_presenter::editing_time_text_range() const {
    if (auto &level = this->_level()) {
        auto const &time_editor = level->editor;
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
    if (auto const settings = this->_nudge_settings.lock()) {
        return settings->unit_index();
    } else {
        return std::nullopt;
    }
}

observing::syncable time_presenter::observe_nudging_unit_index(std::function<void(std::size_t const &)> &&handler) {
    if (auto const settings = this->_nudge_settings.lock()) {
        return settings->observe_unit_index(std::move(handler));
    } else {
        return observing::syncable{};
    }
}

std::shared_ptr<time_editor_level> const &time_presenter::_level() const {
    if (auto const router = this->_project_sub_level_router.lock()) {
        return router->time_editor_level();
    }

    static std::shared_ptr<time_editor_level> const null_level;
    return null_level;
}
