//
//  ae_ui_root_dependency.h
//

#pragma once

#include <memory>

namespace yas::ae {
class ui_editing_root;

struct ui_editing_root_maker_for_ui_root {
    virtual ~ui_editing_root_maker_for_ui_root() = default;

    virtual std::shared_ptr<ui_editing_root> make() const = 0;
};
}  // namespace yas::ae
