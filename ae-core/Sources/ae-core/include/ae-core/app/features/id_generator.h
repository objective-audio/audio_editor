//
//  id_generator.h
//

#pragma once

#include <cpp-utils/identifier.h>

namespace yas::ae {
struct id_generatable {
    virtual ~id_generatable() = default;

    [[nodiscard]] virtual identifier generate() const = 0;
};

struct id_generator final : id_generatable {
    id_generator() = default;

    identifier generate() const;

   private:
    id_generator(id_generator const &) = delete;
    id_generator(id_generator &&) = delete;
    id_generator &operator=(id_generator const &) = delete;
    id_generator &operator=(id_generator &&) = delete;
};
}  // namespace yas::ae
