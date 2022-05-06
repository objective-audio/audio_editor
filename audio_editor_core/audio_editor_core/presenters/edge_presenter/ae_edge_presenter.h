//
//  ae_edge_presenter.h
//

#pragma once

#include <audio_editor_core/ae_edge_location.h>
#include <audio_editor_core/ae_edge_presenter_dependency.h>
#include <audio_editor_core/ae_file_info.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class display_space;
class edge_holder;

struct edge_presenter final {
    [[nodiscard]] static std::shared_ptr<edge_presenter> make_shared(std::string const &project_id,
                                                                     std::shared_ptr<display_space> const &);

    [[nodiscard]] edge_locations locations() const;
    [[nodiscard]] observing::syncable observe_locations(std::function<void(edge_locations const &)> &&);

   private:
    observing::value::holder_ptr<edge_locations> const _locations;

    file_info const _file_info;
    std::weak_ptr<edge_holder> const _edge_holder;
    std::shared_ptr<display_space> const _display_space;

    observing::canceller_pool _pool;

    edge_presenter(file_info const &, std::shared_ptr<edge_holder> const &, std::shared_ptr<display_space> const &);

    void _update_locations();
};
}  // namespace yas::ae
