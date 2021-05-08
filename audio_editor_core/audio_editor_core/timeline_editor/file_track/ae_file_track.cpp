//
//  ae_file_track.cpp
//

#include "ae_file_track.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<file_track> file_track::make_shared() {
    return std::shared_ptr<file_track>(new file_track{});
}
