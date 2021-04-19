//
//  ae_app.cpp
//

#include "ae_app.h"

#include <audio_editor_core/ae_project.h>
#include <cpp_utils/yas_fast_each.h>

using namespace yas;
using namespace yas::ae;

namespace yas::ae {
struct app_factory : app_factory_interface {
    app_project_interface_ptr make_project(url const &file_url) const override {
        return project::make_shared(file_url);
    }
};
}  // namespace yas::ae

app::app(app_factory_interface_ptr const &factory) : _factory(factory) {
}

void app::add_project(url const &file_url) {
    auto const project = this->_factory->make_project(file_url);

    auto canceller = project
                         ->observe_notification([this, project_id = project->identifier()](auto const &notification) {
                             switch (notification) {
                                 case project::notification::should_close: {
                                     this->_projects->erase(project_id);
                                 } break;
                             }
                         })
                         .end();

    this->_projects->insert_or_replace(project->identifier(), std::make_pair(project, std::move(canceller)));
}

std::vector<app_project_interface_ptr> app::projects() const {
    return to_vector<app_project_interface_ptr>(this->_projects->elements(),
                                                [](auto const &pair) { return pair.second.first; });
}

observing::syncable app::observe_projects(observing::caller<projects_map_t::event>::handler_f &&handler) {
    return this->_projects->observe(std::move(handler));
}

app_ptr app::make_shared() {
    return make_shared(std::make_shared<app_factory>());
}

app_ptr app::make_shared(app_factory_interface_ptr const &factory) {
    return std::shared_ptr<app>(new app{factory});
}
