//
//  ae_project_pool_dependency.h
//

#pragma once

#include <cpp_utils/yas_url.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_pool_uuid_generator_interface {
    virtual ~project_pool_uuid_generator_interface() = default;

    virtual std::string generate() const = 0;
};
}  // namespace yas::ae
