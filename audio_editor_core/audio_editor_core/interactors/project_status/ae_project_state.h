//
//  ae_project_state.h
//

#pragma once

namespace yas::ae {
enum class project_state {
    launching,
    loading,
    editing,
    failure,
    closing,
};
}  // namespace yas::ae