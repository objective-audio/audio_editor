//
//  ae_ui_module_waveforms.h
//

#pragma once

#include <cpp_utils/yas_url.h>
#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class waveform_mesh_importer;
class module_location;
class module_waveforms_presenter;

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared(
        std::shared_ptr<module_waveforms_presenter> const &);

    std::shared_ptr<ui::node> const &node();

    void set_scale(ui::size const &);

    void set_locations(std::vector<std::optional<module_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted);

   private:
    std::shared_ptr<module_waveforms_presenter> const _presenter;
    std::shared_ptr<ui::node> const _node;
    std::optional<float> _width_per_sec = std::nullopt;

    observing::canceller_pool _pool;

    ui_module_waveforms(std::shared_ptr<module_waveforms_presenter> const &);

    void _resize_sub_nodes(std::size_t const);
};
}  // namespace yas::ae