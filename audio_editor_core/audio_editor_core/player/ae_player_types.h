//
//  ae_player_types.h
//

#pragma once

#include <cstdint>

namespace yas::playing {
class renderer;
class coordinator;
}  // namespace yas::playing

namespace yas::audio {
class io_device;
}

namespace yas::ae {
using frame_index_t = int64_t;
}
