//
//  ae_db_utils.cpp
//

#include "ae_db_utils.h"

#include <audio_editor_core/ae_db_constants.h>
#include <db/yas_db_umbrella.h>

using namespace yas;
using namespace yas::ae;
using namespace yas::ae::db_constants;

db::model database_utils::make_model() {
    yas::version version{"0.0.1"};

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
                                                  .not_null = true}},
                                  .relations = {}};

    db::entity_args marker_entity{.name = marker_name::entity,
                                  .attributes = {{.name = marker_name::attribute::frame,
                                                  .type = db::attribute_type::integer,
                                                  .default_value = db::value{0},
                                                  .not_null = true}},
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

    db::entity_args pasting_subject_entity{.name = pasting_subject_name::entity,
                                           .attributes = {{.name = pasting_subject_name::attribute::data,
                                                           .type = db::attribute_type::text,
                                                           .default_value = db::value{""},
                                                           .not_null = true}},
                                           .relations = {}};

    return db::model{db::model_args{.version = std::move(version),
                                    .entities = {std::move(module_entity), std::move(marker_entity),
                                                 std::move(edge_entity), std::move(pasting_subject_entity)},
                                    .indices = {}}};
}
