//
//  ae_project_modal_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_project_id.h>
#include <audio_editor_core/ae_project_modal_sub_lifetime.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
struct project_modal_lifecycle final {
    [[nodiscard]] static std::shared_ptr<project_modal_lifecycle> make_shared(project_id const &);

    project_modal_lifecycle(project_id const &);

    std::optional<project_modal_sub_lifetime> const &current() const;

    void add_time_editor(number_components const &, std::optional<std::size_t> const unit_idx);
    void remove_time_editor();
    std::shared_ptr<time_editor_lifetime> const &time_editor_lifetime() const;

    void add_sheet(sheet_content const &);
    void remove_sheet();
    std::shared_ptr<sheet_lifetime> const &sheet_lifetime() const;

    void add_dialog(dialog_content const);
    void remove_dialog();
    std::shared_ptr<dialog_lifetime> const &dialog_lifetime() const;

    void add_context_menu(context_menu const &);
    void remove_context_menu();
    std::shared_ptr<context_menu_lifetime> const &context_menu_lifetime() const;

    [[nodiscard]] observing::syncable observe(
        std::function<void(std::optional<project_modal_sub_lifetime> const &)> &&);

   private:
    project_id const _project_id;

    observing::value::holder_ptr<std::optional<project_modal_sub_lifetime>> const _current;
};
}  // namespace yas::ae
