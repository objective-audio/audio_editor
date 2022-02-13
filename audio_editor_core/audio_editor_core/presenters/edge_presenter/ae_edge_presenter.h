//
//  ae_edge_presenter.h
//

#pragma once

#include <audio_editor_core/ae_edge_location.h>
#include <audio_editor_core/ae_edge_presenter_dependency.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class display_space;

struct edge_presenter final {
    [[nodiscard]] static std::shared_ptr<edge_presenter> make_shared(std::string const &project_id,
                                                                     std::shared_ptr<display_space> const &);
    [[nodiscard]] static std::shared_ptr<edge_presenter> make_shared(
        std::shared_ptr<project_editor_for_edge_presenter> const &, std::shared_ptr<display_space> const &);

    [[nodiscard]] edge_locations locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(edge_locations const &)> &&);

   private:
    observing::value::holder_ptr<edge_locations> const _locations;

    std::weak_ptr<project_editor_for_edge_presenter> const _editor;
    std::shared_ptr<display_space> const _display_space;

    observing::canceller_pool _pool;

    edge_presenter(std::shared_ptr<project_editor_for_edge_presenter> const &, std::shared_ptr<display_space> const &);

    void _update_locations();
};
}  // namespace yas::ae
