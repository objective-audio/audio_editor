//
//  ae_uuid_generator.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
struct uuid_generatable {
    virtual ~uuid_generatable() = default;

    [[nodiscard]] virtual std::string generate() const = 0;
};

struct uuid_generator final : uuid_generatable {
    uuid_generator() = default;

    [[nodiscard]] std::string generate() const override;

   private:
    uuid_generator(uuid_generator const &) = delete;
    uuid_generator(uuid_generator &&) = delete;
    uuid_generator &operator=(uuid_generator const &) = delete;
    uuid_generator &operator=(uuid_generator &&) = delete;
};
}  // namespace yas::ae
