//
//  ae_id_generator.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct id_generator {
    std::string generate() const;

    static std::shared_ptr<id_generator> make_shared();

   private:
    id_generator() = default;
};
}  // namespace yas::ae
