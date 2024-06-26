//
//  ui_marker_name.hpp
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>
#include <ae-core/project_editing/value_types/marker_content.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;

struct ui_marker_name final {
    [[nodiscard]] static std::unique_ptr<ui_marker_name> make_unique(project_lifetime_id const &,
                                                                     ui::node *parent_node);
    ui_marker_name(std::shared_ptr<ui::font_atlas> const &, ui::node *parent_node);

    [[nodiscard]] std::shared_ptr<ui::node> const &node() const;

    void set_content(marker_content const &);
    void update_content(marker_content const &);
    void reset_content();

    void update_color(ui::color const &selected_color, ui::color const &normal_color);

    [[nodiscard]] ui::region square_region() const;

    void finalize();

   private:
    std::optional<marker_content> _content;

    std::shared_ptr<ui::strings> const _name_strings;

    observing::cancellable_ptr _cancellable;

    ui_marker_name(ui_marker_name const &) = delete;
    ui_marker_name(ui_marker_name &&) = delete;
    ui_marker_name &operator=(ui_marker_name const &) = delete;
    ui_marker_name &operator=(ui_marker_name &&) = delete;
};
}  // namespace yas::ae
