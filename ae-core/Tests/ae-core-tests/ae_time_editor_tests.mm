//
//  ae_time_editor_tests.mm
//

#import <XCTest/XCTest.h>
#include <ae-core/time_editor/features/ae_time_editor.h>

using namespace yas;
using namespace yas::ae;

@interface ae_time_editor_tests : XCTestCase

@end

@implementation ae_time_editor_tests

- (void)test_initial {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 8, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    number_components const expected_components{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}}};

    XCTAssertEqual(editor->editing_components(), expected_components);
    XCTAssertEqual(editor->unit_index(), 2);
}

- (void)test_change_sign {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 8, .value = 5}, {.size = 76, .value = 55}}};

    number_components const expected_false_components{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}}};
    number_components const expected_true_components{
        true, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    std::vector<number_components> called;

    auto canceller = editor
                         ->observe_editing_components(
                             [&called](number_components const &components) { called.push_back(components); })
                         .sync();

    XCTAssertEqual(called.size(), 1);

    editor->change_sign_to_minus();

    XCTAssertEqual(editor->editing_components(), expected_true_components);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), expected_true_components);

    editor->change_sign_to_plus();

    XCTAssertEqual(editor->editing_components(), expected_false_components);
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), expected_false_components);

    editor->change_sign_to_plus();
    XCTAssertEqual(called.size(), 3);

    canceller->cancel();
}

- (void)test_edit_number {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 8, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    std::vector<number_components> called;

    auto canceller = editor
                         ->observe_editing_components(
                             [&called](number_components const &components) { called.push_back(components); })
                         .sync();

    XCTAssertTrue(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(
        called.at(0),
        (number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}}}));

    // 未編集から一桁目を入力
    editor->input_number(1);

    number_components const expected_components_1{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 1}}};
    XCTAssertEqual(editor->editing_components(), expected_components_1);
    XCTAssertTrue(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), expected_components_1);

    // 二桁目を入力
    editor->input_number(2);

    number_components const expected_components_2{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 12}}};
    XCTAssertEqual(editor->editing_components(), expected_components_2);
    XCTAssertFalse(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), expected_components_2);

    // 許容範囲が二桁なので三桁目は無視
    editor->input_number(3);

    XCTAssertEqual(editor->editing_components(), expected_components_2);
    XCTAssertFalse(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 3);

    // 三桁目は無視されたので、削除は二桁目
    editor->delete_number();

    number_components const expected_components_3{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 1}}};
    XCTAssertEqual(editor->editing_components(), expected_components_3);
    XCTAssertTrue(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3), expected_components_3);

    // 一桁目を削除
    editor->delete_number();

    number_components const expected_components_4{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 0}}};
    XCTAssertEqual(editor->editing_components(), expected_components_4);
    XCTAssertTrue(editor->can_input_number());
    XCTAssertFalse(editor->can_delete_number());
    XCTAssertEqual(called.size(), 5);
    XCTAssertEqual(called.at(4), expected_components_4);

    // unitを移動
    editor->set_unit_idx(1);

    number_components const expected_components_5{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 0}}};
    XCTAssertEqual(editor->editing_components(), expected_components_5);
    XCTAssertTrue(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 6);
    XCTAssertEqual(called.at(5), expected_components_5);

    // 未編集から削除
    editor->delete_number();

    number_components const expected_components_6{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 0}, {.size = 100, .value = 0}}};
    XCTAssertEqual(editor->editing_components(), expected_components_6);
    XCTAssertTrue(editor->can_input_number());
    XCTAssertFalse(editor->can_delete_number());
    XCTAssertEqual(called.size(), 7);
    XCTAssertEqual(called.at(6), expected_components_6);

    // 0から入力
    editor->input_number(1);

    number_components const expected_components_7{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 1}, {.size = 100, .value = 0}}};
    XCTAssertEqual(editor->editing_components(), expected_components_7);
    XCTAssertFalse(editor->can_input_number());
    XCTAssertTrue(editor->can_delete_number());
    XCTAssertEqual(called.size(), 8);
    XCTAssertEqual(called.at(7), expected_components_7);

    canceller->cancel();
}

- (void)test_move_to {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 8, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    std::vector<std::size_t> called;

    auto canceller = editor->observe_unit_index([&called](std::size_t const &idx) { called.push_back(idx); }).sync();

    XCTAssertEqual(editor->unit_index(), 2);
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), 2);

    editor->input_number(1);
    editor->set_unit_idx(0);

    number_components const expected_components{
        false, {{.size = 10, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 1}}};
    XCTAssertEqual(editor->editing_components(), expected_components);

    XCTAssertEqual(editor->unit_index(), 0);
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), 0);
}

- (void)test_move_to_next_previous {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 8, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    std::vector<std::size_t> called;

    auto canceller = editor->observe_unit_index([&called](std::size_t const &idx) { called.push_back(idx); }).sync();

    XCTAssertEqual(editor->unit_index(), 2);
    XCTAssertFalse(editor->can_move_to_previous_unit());
    XCTAssertTrue(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 1);
    XCTAssertEqual(called.at(0), 2);

    editor->move_to_next_unit();

    XCTAssertEqual(editor->unit_index(), 1);
    XCTAssertTrue(editor->can_move_to_previous_unit());
    XCTAssertTrue(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 2);
    XCTAssertEqual(called.at(1), 1);

    editor->move_to_next_unit();

    XCTAssertEqual(editor->unit_index(), 0);
    XCTAssertTrue(editor->can_move_to_previous_unit());
    XCTAssertFalse(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 3);
    XCTAssertEqual(called.at(2), 0);

    editor->move_to_next_unit();

    XCTAssertEqual(editor->unit_index(), 0);
    XCTAssertTrue(editor->can_move_to_previous_unit());
    XCTAssertFalse(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 3);

    editor->move_to_previous_unit();

    XCTAssertEqual(editor->unit_index(), 1);
    XCTAssertTrue(editor->can_move_to_previous_unit());
    XCTAssertTrue(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 4);
    XCTAssertEqual(called.at(3), 1);

    editor->move_to_previous_unit();

    XCTAssertEqual(editor->unit_index(), 2);
    XCTAssertFalse(editor->can_move_to_previous_unit());
    XCTAssertTrue(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 5);
    XCTAssertEqual(called.at(4), 2);

    editor->move_to_previous_unit();

    XCTAssertEqual(editor->unit_index(), 2);
    XCTAssertFalse(editor->can_move_to_previous_unit());
    XCTAssertTrue(editor->can_move_to_next_unit());
    XCTAssertEqual(called.size(), 5);

    canceller->cancel();
}

- (void)test_commit_number {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 11, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    editor->move_to_next_unit();
    XCTAssertEqual(editor->unit_index(), 1);

    {
        // size内なのでそのままセット
        editor->input_number(1);
        editor->input_number(0);
        editor->move_to_next_unit();

        XCTAssertEqual(editor->editing_components(),
                       (number_components{
                           false, {{.size = 10, .value = 1}, {.size = 100, .value = 10}, {.size = 100, .value = 55}}}));
    }

    {
        // sizeと同じなので繰り上がって0
        editor->move_to_previous_unit();
        editor->input_number(1);
        editor->input_number(1);
        editor->move_to_next_unit();

        XCTAssertEqual(editor->editing_components(),
                       (number_components{
                           false, {{.size = 10, .value = 1}, {.size = 100, .value = 0}, {.size = 100, .value = 56}}}));
    }

    {
        // sizeより大きいので繰り上がって余りをセット
        editor->move_to_previous_unit();
        editor->input_number(1);
        editor->input_number(2);
        editor->move_to_next_unit();

        XCTAssertEqual(editor->editing_components(),
                       (number_components{
                           false, {{.size = 10, .value = 1}, {.size = 100, .value = 1}, {.size = 100, .value = 57}}}));
    }
}

- (void)test_finalized_components {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 11, .value = 5}, {.size = 76, .value = 55}}};

    auto const editor = std::make_shared<ae::time_editor>(components);

    std::vector<time_editor_event> called;

    editor->input_number(9);
    editor->input_number(9);

    XCTAssertEqual(
        editor->editing_components(),
        (number_components{false, {{.size = 10, .value = 1}, {.size = 100, .value = 5}, {.size = 100, .value = 99}}}));

    editor->commit_unit_value();

    XCTAssertEqual(
        editor->finalized_components(),
        (number_components{false, {{.size = 2, .value = 1}, {.size = 11, .value = 5}, {.size = 76, .value = 75}}}));
}

@end
