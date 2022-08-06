//
//  ae_app_lifetime.h
//

#pragma once

#include <cpp_utils/yas_worker.h>

namespace yas::ae {
class app_launcher;
class window_lifecycle;
class system_url;
class file_importer;
class file_info_loader;
class color;
class ui_resource_lifecycle;
class ui_root_lifecycle;
class window_opener;
class app_modal_lifecycle;
class window_lifecycle;
class action_sender;
class uuid_generator;
class id_generator;

struct app_lifetime final {
    [[nodiscard]] static std::shared_ptr<app_lifetime> make_shared();

    app_lifetime(std::shared_ptr<worker> const &, std::shared_ptr<system_url> const &,
                 std::shared_ptr<app_launcher> const &, std::shared_ptr<file_importer> const &,
                 std::shared_ptr<file_info_loader> const &, std::shared_ptr<color> const &,
                 std::shared_ptr<uuid_generator> const &, std::shared_ptr<id_generator> const &,
                 std::shared_ptr<window_lifecycle> const &, std::shared_ptr<app_modal_lifecycle> const &,
                 std::shared_ptr<ui_resource_lifecycle> const &, std::shared_ptr<window_opener> const &,
                 std::shared_ptr<action_sender> const &);

    std::shared_ptr<worker> const worker;
    std::shared_ptr<system_url> const system_url;
    std::shared_ptr<app_launcher> const launcher;
    std::shared_ptr<file_importer> const file_importer;
    std::shared_ptr<file_info_loader> const file_info_loader;
    std::shared_ptr<color> const color;
    std::shared_ptr<uuid_generator> const uuid_generator;
    std::shared_ptr<id_generator> const id_generator;

    std::shared_ptr<window_lifecycle> const window_lifecycle;
    std::shared_ptr<app_modal_lifecycle> const modal_lifecycle;
    std::shared_ptr<ui_resource_lifecycle> const ui_resource_lifecycle;

    std::shared_ptr<window_opener> const window_opener;

    std::shared_ptr<action_sender> const action_sender;
};
}  // namespace yas::ae
