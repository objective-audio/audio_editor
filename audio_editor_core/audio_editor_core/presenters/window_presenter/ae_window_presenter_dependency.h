//
//  ae_window_presenter_dependency.h
//

#pragma once

namespace yas::ae {
struct project_for_window_presenter {
    virtual ~project_for_window_presenter() = default;

    [[nodiscard]] virtual std::string const &identifier() const = 0;
    [[nodiscard]] virtual url const &file_url() const = 0;

    [[nodiscard]] virtual bool can_close() const = 0;
    virtual void request_close() = 0;
};
}  // namespace yas::ae
