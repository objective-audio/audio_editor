//
//  ae_ptr.h
//

#pragma once

#include <memory>

namespace yas::ae {
class app;
class app_project_interface;
class project;

using app_ptr = std::shared_ptr<app>;
using app_project_interface_ptr = std::shared_ptr<app_project_interface>;
using project_ptr = std::shared_ptr<project>;
using project_wptr = std::weak_ptr<project>;
}  // namespace yas::ae
