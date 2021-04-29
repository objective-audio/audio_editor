//
//  ae_uuid_generator.cpp
//

#include "ae_uuid_generator.h"

#include <uuid/uuid.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<uuid_generator> uuid_generator::make_shared() {
    return std::shared_ptr<uuid_generator>(new uuid_generator{});
}

std::string uuid_generator::generate() const {
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_string_t string;
    uuid_unparse_upper(uuid, string);
    return string;
}
