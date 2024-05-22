//
//  ae_common_types.h
//

#pragma once

#include <audio-processing/common/common_types.h>
#include <audio-processing/time/time.h>
#include <db/additions/object/object_id.h>

namespace yas::ae {
using frame_index_t = proc::frame_index_t;
using length_t = proc::length_t;
using sample_rate_t = proc::sample_rate_t;
using track_index_t = proc::track_index_t;
using time = proc::time;

using object_id = db::object_id;
}  // namespace yas::ae
