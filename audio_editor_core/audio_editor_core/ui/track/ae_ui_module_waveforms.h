//
//  ae_ui_module_waveforms.h
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
class waveform_mesh_importer;
class module_location;

struct ui_module_waveforms final {
    [[nodiscard]] static std::shared_ptr<ui_module_waveforms> make_shared();

    std::shared_ptr<ui::node> const &node();

    void set_locations(std::vector<std::optional<module_location>> const &);
    void update_locations(std::size_t const count, std::vector<std::pair<std::size_t, module_location>> const &erased,
                          std::vector<std::pair<std::size_t, module_location>> const &inserted);

   private:
    std::shared_ptr<waveform_mesh_importer> const _mesh_importer;
    std::shared_ptr<ui::node> const _node;

    observing::canceller_pool _pool;

    ui_module_waveforms();

    void _resize_sub_nodes(std::size_t const);
};
}  // namespace yas::ae
