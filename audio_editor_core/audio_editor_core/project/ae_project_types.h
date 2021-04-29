//
//  ae_project_types.h
//

#pragma once

namespace yas::ae {
enum class project_event {
    should_close,
};

enum class project_state {
    launching,
    loading,
    editing,
    failure,
    closing,
};
}  // namespace yas::ae
