//
//  ae_app_dialog_level.cpp
//

#include "ae_app_dialog_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<app_dialog_level> app_dialog_level::make_shared(app_dialog_content const content) {
    return std::make_shared<app_dialog_level>(content);
}

app_dialog_level::app_dialog_level(app_dialog_content const content) : content(content) {
}
