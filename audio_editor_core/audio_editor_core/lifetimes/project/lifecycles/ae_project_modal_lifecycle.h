//
//  ae_project_modal_lifecycle.h
//

#pragma once

#include <audio_editor_core/ae_action_receiver_providable.h>
#include <audio_editor_core/ae_project_modal_sub_lifetime.h>
#include <audio_editor_core/ae_window_lifetime_id.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
class id_generatable;

struct project_modal_lifecycle final : action_receiver_providable {
    [[nodiscard]] static std::shared_ptr<project_modal_lifecycle> make_shared(window_lifetime_id const &);

    project_modal_lifecycle(id_generatable const *, window_lifetime_id const &);

    std::optional<project_modal_sub_lifetime> const &current() const;

    void add_time_editor(number_components const &);
    void remove_time_editor(time_editor_lifetime_id const &);
    std::shared_ptr<time_editor_lifetime> const &time_editor_lifetime() const;

    void add_module_name_sheet(time::range const &);
    void remove_module_name_sheet(sheet_lifetime_id const &);
    std::shared_ptr<module_name_sheet_lifetime> const &module_name_sheet_lifetime() const;

    void add_marker_name_sheet(int64_t const);
    void remove_marker_name_sheet(sheet_lifetime_id const &);
    std::shared_ptr<marker_name_sheet_lifetime> const &marker_name_sheet_lifetime() const;

    void add_file_import_dialog();
    void remove_file_import_dialog(dialog_lifetime_id const &);
    std::shared_ptr<file_import_dialog_lifetime> const &file_import_dialog_lifetime() const;

    void add_file_export_dialog();
    void remove_file_export_dialog(dialog_lifetime_id const &);
    std::shared_ptr<file_export_dialog_lifetime> const &file_export_dialog_lifetime() const;

    void add_context_menu(context_menu const &);
    void remove_context_menu(context_menu_lifetime_id const &);
    std::shared_ptr<context_menu_lifetime> const &context_menu_lifetime() const;

    [[nodiscard]] observing::syncable observe(
        std::function<void(std::optional<project_modal_sub_lifetime> const &)> &&);

   private:
    id_generatable const *const _id_generator;
    window_lifetime_id const _window_lifetime_id;

    observing::value::holder_ptr<std::optional<project_modal_sub_lifetime>> const _current;

#pragma mark - action_receiver_provider

    std::optional<action_id> receivable_id() const override;
    std::vector<action_receivable *> receivers() const override;
    std::vector<action_receiver_providable *> sub_providers() const override;
};
}  // namespace yas::ae
