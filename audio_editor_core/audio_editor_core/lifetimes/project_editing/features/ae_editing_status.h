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

    editing_status(editing_status const &) = delete;
    editing_status(editing_status &&) = delete;
    editing_status &operator=(editing_status const &) = delete;
    editing_status &operator=(editing_status &&) = delete;
};
}  // namespace yas::ae
