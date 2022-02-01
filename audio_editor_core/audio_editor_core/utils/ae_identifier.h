//
//  ae_identifier.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct identifier final {
    identifier();
    ~identifier();

    identifier(identifier const &);
    identifier(identifier &&);
    identifier &operator=(identifier const &);
    identifier &operator=(identifier &&);

    bool operator==(identifier const &rhs) const;
    bool operator!=(identifier const &rhs) const;
    bool operator<(identifier const &) const;

    uintptr_t raw_value() const;

   private:
    std::shared_ptr<std::nullptr_t> _raw;
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::identifier const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::identifier const &value);
