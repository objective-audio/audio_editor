//
//  ae_ui_time_numbers.hpp
//

#pragma once

#include <ae-core/global/value_types/ae_project_lifetime_id.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class color;
class time_numbers_presenter;

struct ui_time_numbers final {
    [[nodiscard]] static std::shared_ptr<ui_time_numbers> make_shared(project_lifetime_id const &,
                                                                      std::shared_ptr<ui::node> const &);
    ui_time_numbers(std::shared_ptr<time_numbers_presenter> const &, std::shared_ptr<ui::font_atlas> const &,
                    std::shared_ptr<ui::standard> const &, ae::color *, std::shared_ptr<ui::node> const &);

    [[nodiscard]] std::shared_ptr<ui::layout_region_source> actual_layout_source() const;
    [[nodiscard]] std::optional<ui::region> button_region(index_range const &range) const;

    [[nodiscard]] observing::syncable observe_button_regions(std::function<void(void)> &&);

   private:
    std::shared_ptr<time_numbers_presenter> const _presenter;
    std::shared_ptr<ui::strings> const _strings;
    ae::color *const _color;
    observing::canceller_pool _pool;

    ui_time_numbers(ui_time_numbers const &) = delete;
    ui_time_numbers(ui_time_numbers &&) = delete;
    ui_time_numbers &operator=(ui_time_numbers const &) = delete;
    ui_time_numbers &operator=(ui_time_numbers &&) = delete;

    void _update_time_strings_attributes();
};
}  // namespace yas::ae
