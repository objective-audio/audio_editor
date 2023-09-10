//
//  ae_tracks_controller.cpp
//

#include "ae_tracks_controller.hpp"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<tracks_controller> tracks_controller::make_shared(project_lifetime_id const &project_lifetime_id) {
    return std::make_shared<tracks_controller>(project_lifetime_id);
}

tracks_controller::tracks_controller(project_lifetime_id const &) {
}
