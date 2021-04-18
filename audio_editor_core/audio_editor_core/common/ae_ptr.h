//
//  ae_ptr.h
//

#pragma once

#include <memory>

namespace yas::ae {
class app;
class project;

using app_ptr = std::shared_ptr<app>;
using project_ptr = std::shared_ptr<project>;
using project_wptr = std::weak_ptr<project>;
}  // namespace yas::ae
