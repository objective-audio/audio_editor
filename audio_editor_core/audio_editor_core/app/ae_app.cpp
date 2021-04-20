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

app_project_interface_ptr app::add_project(url const &file_url) {
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

    return project;
}

app_project_interface_ptr app::project_for_id(uintptr_t const project_id) const {
    if (this->_projects->contains(project_id)) {
        return this->_projects->at(project_id).first;
    } else {
        return nullptr;
    }
}

std::vector<app_project_interface_ptr> app::projects() const {
    return to_vector<app_project_interface_ptr>(this->_projects->elements(),
                                                [](auto const &pair) { return pair.second.first; });
}

observing::syncable app::observe_project(std::function<void(app_projects_event const &)> &&handler) {
    return this->_projects->observe([handler = std::move(handler)](app::projects_map_t::event const &event) {
        switch (event.type) {
            case observing::map::event_type::any: {
                for (auto const &pair : event.elements) {
                    handler({.type = app_projects_event_type::inserted, .project_id = pair.second.first->identifier()});
                }
            } break;
            case observing::map::event_type::inserted: {
                handler({.type = app_projects_event_type::inserted, .project_id = event.inserted->first->identifier()});
            } break;
            case observing::map::event_type::replaced: {
                handler({.type = app_projects_event_type::erased, .project_id = event.erased->first->identifier()});
                handler({.type = app_projects_event_type::inserted, .project_id = event.inserted->first->identifier()});
            } break;
            case observing::map::event_type::erased: {
                handler({.type = app_projects_event_type::erased, .project_id = event.erased->first->identifier()});
            } break;
        }
    });
}

app_ptr app::make_shared() {
    return make_shared(std::make_shared<app_factory>());
}

app_ptr app::make_shared(app_factory_interface_ptr const &factory) {
    return std::shared_ptr<app>(new app{factory});
}
