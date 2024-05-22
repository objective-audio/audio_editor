//
//  ae_app_lifetime_types.h
//

#pragma once

#include <cstdint>

namespace yas::ae {
enum class worker_priority : uint32_t {
    file_importing,
};
}  // namespace yas::ae
