//
//  ae_ui_scroller_nodes.hpp
//

#pragma once

#include <ui/yas_ui_umbrella.h>

namespace yas::ae {
struct ui_scroller_nodes {
    std::shared_ptr<ui::node> const back;
    std::shared_ptr<ui::node> const modules;
    std::shared_ptr<ui::node> const front;
};
}  // namespace yas::ae
