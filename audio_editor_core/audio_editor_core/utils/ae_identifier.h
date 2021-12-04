//
//  ae_identifier.h
//

#pragma once

#include <memory>

namespace yas::ae {
struct identifier final {
    identifier();

    identifier(identifier const &);
    identifier(identifier &&);
    identifier &operator=(identifier const &);
    identifier &operator=(identifier &&);

    bool operator==(identifier const &rhs) const;
    bool operator!=(identifier const &rhs) const;

    uintptr_t raw_value() const;

   private:
    std::shared_ptr<std::nullptr_t> _raw;
};
}  // namespace yas::ae
