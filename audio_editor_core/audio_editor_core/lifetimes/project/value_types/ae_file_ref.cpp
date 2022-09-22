//
//  ae_file_ref.cpp
//

#include "ae_file_ref.hpp"

using namespace yas;
using namespace yas::ae;

file_ref_object::file_ref_object(object_id const &identifier, file_ref &&params)
    : file_ref_object(identifier, params.file_name) {
}

file_ref_object::file_ref_object(object_id const &identifier, std::string const &file_name)
    : identifier(identifier), file_name(file_name) {
}
