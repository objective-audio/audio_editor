//
//  ae_ui_scroller_nodes.hpp
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_scroller_nodes {
    std::shared_ptr<ui::node> const back;
    std::shared_ptr<ui::node> const modules;
    std::shared_ptr<ui::node> const front_y_fixed;
    std::shared_ptr<ui::node> const front_x_fixed;
};
}  // namespace yas::ae
