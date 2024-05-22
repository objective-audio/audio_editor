//
//  ae_project_setup_dialog_opener.hpp
//

#pragma once

namespace yas::ae {
class app_modal_lifecycle;

struct project_setup_dialog_opener final {
    project_setup_dialog_opener(app_modal_lifecycle *);

    [[nodiscard]] bool can_open() const;
    void open();

   private:
    app_modal_lifecycle *const _app_modal_lifecycle;

    project_setup_dialog_opener(project_setup_dialog_opener const &) = delete;
    project_setup_dialog_opener(project_setup_dialog_opener &&) = delete;
    project_setup_dialog_opener &operator=(project_setup_dialog_opener const &) = delete;
    project_setup_dialog_opener &operator=(project_setup_dialog_opener &&) = delete;
};
}  // namespace yas::ae
