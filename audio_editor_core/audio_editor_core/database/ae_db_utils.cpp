//
//  ae_db_utils.cpp
//

#include "ae_db_utils.h"

#include <audio_editor_core/ae_db_constants.h>
#include <db/yas_db_umbrella.h>

using namespace yas;
using namespace yas::ae;

db::model database_utils::make_model() {
    yas::version version{"0.0.1"};

    db::entity_args module_entity{.name = db_constants::module_name::entity,
                                  .attributes = {{.name = db_constants::module_name::file_frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = db_constants::module_name::range_frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = db_constants::module_name::range_length,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true}},
                                  .relations = {}};

    db::entity_args marker_entity{.name = db_constants::marker_name::entity,
                                  .attributes = {{.name = db_constants::marker_name::frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true}},
                                  .relations = {}};

    return db::model{db::model_args{.version = std::move(version),
                                    .entities = {std::move(module_entity), std::move(marker_entity)},
                                    .indices = {}}};
}
