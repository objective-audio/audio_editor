//
//  ae_time_editor_level_pool.h
//

#pragma once

#include <memory>
#include <string>

namespace yas::ae {
class time_editor_level;
class number_components;

struct time_editor_level_pool final {
    [[nodiscard]] static std::shared_ptr<time_editor_level_pool> make_shared(std::string const &identifier);

    void add_level(number_components const &);
    std::shared_ptr<time_editor_level> const &level();

   private:
    std::string const _identifier;

    std::shared_ptr<time_editor_level> _level;

    time_editor_level_pool(std::string const &identifier);
};
}  // namespace yas::ae
