//
//  ae_project_pool_dependency.h
//

#pragma once

#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct uuid_generator_for_project_pool {
    virtual ~uuid_generator_for_project_pool() = default;

    virtual std::string generate() const = 0;
};
}  // namespace yas::ae
