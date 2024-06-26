//
//  db_utils.cpp
//

#include "db_utils.h"

#include <ae-core/project_editing/value_types/db_constants.h>

#include <db/umbrella.hpp>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db::model database_utils::make_model() {
    yas::version version{"0.0.5"};

    db::entity_args module_entity{.name = module_name::entity,
                                  .attributes = {{.name = module_name::attribute::name,
                                                  .type = db::attribute_type::text,
                                                  .default_value = db::value{""},
                                                  .not_null = true},
                                                 {.name = module_name::attribute::file_frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = module_name::attribute::file_name,
                                                  .type = db::attribute_type::text,
                                                  .default_value = db::value{""},
                                                  .not_null = true},
                                                 {.name = module_name::attribute::range_frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = module_name::attribute::range_length,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = module_name::attribute::track,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true}},
                                  .relations = {}};

    db::entity_args marker_entity{.name = marker_name::entity,
                                  .attributes = {{.name = marker_name::attribute::frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true},
                                                 {.name = marker_name::attribute::name,
                                                  .type = db::attribute_type::text,
                                                  .default_value = db::value{""}}},
                                  .relations = {}};

    db::entity_args edge_entity{.name = edge_name::entity,
                                .attributes = {{.name = edge_name::attribute::begin,
                                                .type = db::attribute_type::integer,
                                                .default_value = db::value{0},
                                                .not_null = true},
                                               {.name = edge_name::attribute::end,
                                                .type = db::attribute_type::integer,
                                                .default_value = db::value{0},
                                                .not_null = true}},
                                .relations = {}};

    return db::model{
        db::model_args{.version = std::move(version),
                       .entities = {std::move(module_entity), std::move(marker_entity), std::move(edge_entity)},
                       .indices = {}}};
}
