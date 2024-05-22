//
//  ae_timeline_updater.cpp
//

#include "ae_timeline_updater.h"

#include <ae-core/project/features/ae_timeline_holder.h>
#include <ae-core/project_editing/features/ae_module_pool.h>

using namespace yas;
using namespace yas::ae;

timeline_updater::timeline_updater(module_pool *module_pool, timeline_holder *timeline_holder)
    : _module_pool(module_pool), _timeline_holder(timeline_holder) {
    this->_module_pool
        ->observe_event([this](module_pool_event const &event) {
            switch (event.type) {
                case module_pool_event_type::any: {
                    this->_timeline_holder->replace(event.modules);
                } break;
                case module_pool_event_type::reverted: {
                    this->_timeline_holder->replace(event.modules);
                } break;
                case module_pool_event_type::inserted: {
                    auto const &module = event.module.value().value;
                    this->_timeline_holder->insert(module);
                } break;
                case module_pool_event_type::erased: {
                    auto const &module = event.module.value().value;
                    this->_timeline_holder->erase(module.track, module.range);
                } break;
                case module_pool_event_type::detail_updated:
                    break;
            }
        })
        .sync()
        ->add_to(this->_pool);
}
