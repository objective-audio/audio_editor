//
//  db_utils.h
//

#pragma once

namespace yas::db {
class model;
}

namespace yas::ae::database_utils {
[[nodiscard]] db::model make_model();
}
