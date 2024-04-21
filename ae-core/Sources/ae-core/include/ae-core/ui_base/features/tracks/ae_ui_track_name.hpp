#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

#include <ae-core/project_editing/value_types/ae_track_content.hpp>

namespace yas::ae {
struct ui_track_name {
    [[nodiscard]] static std::unique_ptr<ui_track_name> make_unique(project_lifetime_id const &, ui::node *parent_node);
    ui_track_name(std::shared_ptr<ui::font_atlas> const &, ui::node *parent_node);

    [[nodiscard]] std::shared_ptr<ui::node> const &node() const;

    void set_content(track_content const &);
    void update_content(track_content const &);
    void reset_content();

    void update_color(ui::color const &selected_color, ui::color const &normal_color);

    [[nodiscard]] ui::region square_region() const;

    void finalize();

   private:
    std::optional<track_content> _content;

    std::shared_ptr<ui::strings> const _name_strings;

    observing::cancellable_ptr _cancellable;

    ui_track_name(ui_track_name const &) = delete;
    ui_track_name(ui_track_name &&) = delete;
    ui_track_name &operator=(ui_track_name const &) = delete;
    ui_track_name &operator=(ui_track_name &&) = delete;
};
}  // namespace yas::ae
