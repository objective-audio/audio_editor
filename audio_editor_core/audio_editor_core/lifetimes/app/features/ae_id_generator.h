//
//  ae_id_generator.h
//

#pragma once

#include <cpp_utils/yas_identifier.h>

namespace yas::ae {
struct id_generatable {
    virtual ~id_generatable() = default;

    [[nodiscard]] virtual identifier generate() const = 0;
};

struct id_generator final : id_generatable {
    [[nodiscard]] static std::shared_ptr<id_generator> make_shared();

    identifier generate() const;
};
}  // namespace yas::ae
