//
//  ae_uuid_generator.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct uuid_generator {
    std::string generate() const;

    static std::shared_ptr<uuid_generator> make_shared();

   private:
    uuid_generator() = default;
};
}  // namespace yas::ae
