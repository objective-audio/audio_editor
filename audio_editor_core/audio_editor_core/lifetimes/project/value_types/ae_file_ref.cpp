//
//  ae_file_ref.cpp
//

#include "ae_file_ref.hpp"

using namespace yas;
using namespace yas::ae;

file_ref_object::file_ref_object(object_id const &identifier, file_ref &&value)
    : identifier(identifier), value(std::move(value)) {
}
