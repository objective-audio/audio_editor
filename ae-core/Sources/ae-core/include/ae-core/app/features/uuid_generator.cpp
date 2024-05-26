//
//  uuid_generator.cpp
//

#include "uuid_generator.h"

#include <uuid/uuid.h>

using namespace yas;
using namespace yas::ae;

std::string uuid_generator::generate() const {
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_string_t string;
    uuid_unparse_upper(uuid, string);
    return string;
}
