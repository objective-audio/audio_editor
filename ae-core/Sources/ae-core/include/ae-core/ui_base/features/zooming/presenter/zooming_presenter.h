//
//  zooming_presenter.h
//

#pragma once

#include <ae-core/app/value_types/file_info.h>
#include <ae-core/global/value_types/project_lifetime_id.h>

#include <ae-core/project/value_types/zooming_types.hpp>
#include <observing/umbrella.hpp>

namespace yas::ae {
class player;
class zooming_pair;

struct zooming_presenter final {
    [[nodiscard]] static std::shared_ptr<zooming_presenter> make_shared(project_lifetime_id const &);

    zooming_presenter(std::shared_ptr<zooming_pair> const &);

    [[nodiscard]] ae::zooming_scale zooming_scale() const;
    [[nodiscard]] observing::syncable observe_zooming_scale(std::function<void(ae::zooming_scale const &)> &&);

   private:
    std::weak_ptr<zooming_pair> const _zooming_pair;

    zooming_presenter(zooming_presenter const &) = delete;
    zooming_presenter(zooming_presenter &&) = delete;
    zooming_presenter &operator=(zooming_presenter const &) = delete;
    zooming_presenter &operator=(zooming_presenter &&) = delete;
};
}  // namespace yas::ae
