//
//  project_opener_event.h
//

#pragma once

#include <ae-core/global/value_types/project_lifetime_id.h>

namespace yas::ae {
enum class project_opener_event_kind {
    show_opened,
};

struct project_opener_event {
    project_opener_event_kind kind;
    project_lifetime_id lifetime_id;
};
}  // namespace yas::ae
