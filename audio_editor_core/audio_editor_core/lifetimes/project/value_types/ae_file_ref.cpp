//
//  ae_file_ref.cpp
//

#include "ae_file_ref.hpp"

using namespace yas;
using namespace yas::ae;

file_ref::file_ref(object_id const &identifier, params &&params) : file_ref(identifier, params.file_name) {
}

file_ref::file_ref(object_id const &identifier, std::string const &file_name)
    : identifier(identifier), file_name(file_name) {
}

[[nodiscard]] file_ref::params file_ref::parameters() const {
    return {this->file_name};
}
