//
//  ae_project.h
//

#pragma once

#include <memory>

namespace yas::ae {
class project;
using project_ptr = std::shared_ptr<project>;

struct project {
    static project_ptr make_shared();

   private:
    project();

    project(project const &) = delete;
    project(project &&) = delete;
    project &operator=(project const &) = delete;
    project &operator=(project &&) = delete;
};
}  // namespace yas::ae
