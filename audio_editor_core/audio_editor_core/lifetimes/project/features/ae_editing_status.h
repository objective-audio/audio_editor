//
//  ae_editing_status.h
//

#pragma once

#include <memory>

namespace yas::ae {
class exporter;

struct editing_status final {
    editing_status(exporter const *);

    [[nodiscard]] bool can_editing() const;

   private:
    exporter const *const _exporter;
};
}  // namespace yas::ae
