//
//  ae_edge_presenter.h
//

#pragma once

#include <audio_editor_core/ae_edge_location.h>
#include <audio_editor_core/ae_edge_presenter_dependency.h>
#include <audio_editor_core/ae_project_format.h>
#include <audio_editor_core/ae_project_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class display_space;
class edge_holder;

struct edge_presenter final {
    [[nodiscard]] static std::shared_ptr<edge_presenter> make_shared(project_lifetime_id const &);

    edge_presenter(project_format const &, std::shared_ptr<edge_holder> const &,
                   std::shared_ptr<display_space> const &);

    [[nodiscard]] edge_locations locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(edge_locations const &)> &&);

   private:
    observing::value::holder_ptr<edge_locations> const _locations;

    project_format const _project_format;
    std::weak_ptr<edge_holder> const _edge_holder;
    std::weak_ptr<display_space> const _display_space;

    observing::canceller_pool _pool;

    edge_presenter(edge_presenter const &) = delete;
    edge_presenter(edge_presenter &&) = delete;
    edge_presenter &operator=(edge_presenter const &) = delete;
    edge_presenter &operator=(edge_presenter &&) = delete;

    void _update_locations();
};
}  // namespace yas::ae
