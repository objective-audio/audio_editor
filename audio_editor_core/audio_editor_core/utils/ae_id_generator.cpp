//
//  ae_id_generator.cpp
//

#include "ae_id_generator.h"

#include <uuid/uuid.h>

using namespace yas;
using namespace yas::ae;

std::shared_ptr<id_generator> id_generator::make_shared() {
    return std::shared_ptr<id_generator>(new id_generator{});
}

std::string id_generator::generate() const {
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_string_t string;
    uuid_unparse_upper(uuid, string);
    return string;
}
