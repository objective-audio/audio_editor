//
//  ae_edge_presenter_dependency.h
//

#pragma once

#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class file_info;
class edge;
class edge_editor_event;

struct project_editor_for_edge_presenter {
    virtual ~project_editor_for_edge_presenter() = default;

    [[nodiscard]] virtual ae::edge const &edge() const = 0;
    [[nodiscard]] virtual observing::syncable observe_edge_editor_event(
        std::function<void(edge_editor_event const &)> &&) = 0;
};
}  // namespace yas::ae
