//
//  ae_dialog_lifetime.cpp
//

#include "ae_dialog_lifetime.h"

using namespace yas;
using namespace yas::ae;

std::shared_ptr<dialog_lifetime> dialog_lifetime::make_shared(dialog_content const content) {
    return std::make_shared<dialog_lifetime>(content);
}

dialog_lifetime::dialog_lifetime(dialog_content const content) : content(content) {
}
