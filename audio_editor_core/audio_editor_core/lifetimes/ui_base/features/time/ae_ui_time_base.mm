//
//  ae_ui_time_base.cpp
//

#include "ae_ui_time_base.hpp"
#include <audio_editor_core/ae_ui_hierarchy.h>
#include <audio_editor_core/ae_ui_layout_utils.h>
#include <audio_editor_core/ae_ui_time_constants.h>
#include <audio_editor_core/ae_ui_resource_lifetime.hpp>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<ui_time_base> ui_time_base::make_shared(window_lifetime_id const &lifetime_id,
                                                        std::shared_ptr<ui::node> const &time_base_node) {
    auto const &resource_lifetime = ui_hierarchy::resource_lifetime_for_window_lifetime_id(lifetime_id);

    return std::make_shared<ui_time_base>(time_base_node, resource_lifetime->time_font_atlas,
                                          resource_lifetime->standard);
}

ui_time_base::ui_time_base(std::shared_ptr<ui::node> const &time_base_node,
                           std::shared_ptr<ui::font_atlas> const &time_font_atlas,
                           std::shared_ptr<ui::standard> const &standard)
    : _top_guide(ui::layout_value_guide::make_shared()) {
    float const time_height = ui_time_constants::nudge_height + time_font_atlas->ascent() + time_font_atlas->descent();

    ui::layout(standard->view_look()->view_layout_guide()->bottom(), this->_top_guide,
               ae::ui_layout_utils::constant(time_height))
        .sync()
        ->add_to(this->_pool);

    time_base_node->attach_y_layout_guide(*this->_top_guide);
}
