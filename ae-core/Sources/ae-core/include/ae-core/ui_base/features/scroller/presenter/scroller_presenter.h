//
//  scroller_presenter.h
//

#pragma once

#include <ae-core/app/value_types/project_format.h>
#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ui/common/yas_ui_types.h>

#include <ae-core/project/value_types/zooming_types.hpp>
#include <memory>

namespace yas::ae {
class player;
class vertical_scrolling;
class zooming_pair;

struct scroller_presenter final {
    [[nodiscard]] static std::shared_ptr<scroller_presenter> make_shared(project_lifetime_id const &);

    scroller_presenter(project_format const &, std::shared_ptr<player> const &,
                       std::shared_ptr<vertical_scrolling> const &, std::shared_ptr<zooming_pair> const &);

    [[nodiscard]] ui::point modules_position() const;
    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

   private:
    project_format const _project_format;
    std::weak_ptr<player> _player;
    std::weak_ptr<vertical_scrolling> _vertical_scrolling;
    std::weak_ptr<zooming_pair> _zooming_pair;

    scroller_presenter(scroller_presenter const &) = delete;
    scroller_presenter(scroller_presenter &&) = delete;
    scroller_presenter &operator=(scroller_presenter const &) = delete;
    scroller_presenter &operator=(scroller_presenter &&) = delete;
};
}  // namespace yas::ae
