//
//  ae_editing_status.cpp
//

#include "ae_editing_status.h"

#include <audio_editor_core/ae_exporter.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<editing_status> editing_status::make_shared(exporter const *exporter) {
    return std::shared_ptr<editing_status>(new editing_status{exporter});
}

editing_status::editing_status(exporter const *exporter) : _exporter(exporter) {
}

bool editing_status::can_editing() const {
    return !this->_exporter->is_exporting();
}
