//
//  ae_uuid_generator.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct uuid_generatable {
    virtual ~uuid_generatable() = default;

    virtual std::string generate() const = 0;
};

struct uuid_generator final : uuid_generatable {
    std::string generate() const override;

    static std::shared_ptr<uuid_generator> make_shared();

   private:
    uuid_generator() = default;
};
}  // namespace yas::ae
