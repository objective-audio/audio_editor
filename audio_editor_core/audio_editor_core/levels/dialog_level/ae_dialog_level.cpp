//
//  ae_dialog_level.cpp
//

#include "ae_dialog_level.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<dialog_level> dialog_level::make_shared(dialog_content const content) {
    return std::make_shared<dialog_level>(content);
}

dialog_level::dialog_level(dialog_content const content) : content(content) {
}
