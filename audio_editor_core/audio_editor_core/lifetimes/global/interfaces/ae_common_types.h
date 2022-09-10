//
//  ae_common_types.h
//

#pragma once

#include <db/yas_db_object_id.h>
#include <processing/yas_processing_common_types.h>
#include <processing/yas_processing_time.h>

namespace yas::ae {
using frame_index_t = proc::frame_index_t;
using length_t = proc::length_t;
using sample_rate_t = proc::sample_rate_t;
using time = proc::time;

using object_id = db::object_id;
}  // namespace yas::ae
