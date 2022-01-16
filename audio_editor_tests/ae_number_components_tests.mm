//
//  ae_number_components_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_number_components.h>

using namespace yas;
using namespace yas::ae;

@interface ae_number_components_tests : XCTestCase

@end

@implementation ae_number_components_tests

- (void)test_initial {
    number_components const components{
        false, {{.count = 2, .value = 1}, {.count = 10, .value = 5}, {.count = 100, .value = 55}}};

    std::vector<number_components::unit> const expected_units{
        {.count = 2, .value = 1}, {.count = 10, .value = 5}, {.count = 100, .value = 55}};

    XCTAssertFalse(components.is_minus());

    XCTAssertEqual(components.units_count(), 3);
    XCTAssertEqual(components.unit_value(0), 1);
    XCTAssertEqual(components.unit_value(1), 5);
    XCTAssertEqual(components.unit_value(2), 55);
}

- (void)test_constructor_throws {
    XCTAssertThrows((number_components{false, {}}), @"unitsが無い");
    XCTAssertThrows((number_components{false, {{.count = 10, .value = 10}, {.count = 10, .value = 0}}}),
                    @"valueがcount以上");
    XCTAssertThrows((number_components{false, {{.count = 10, .value = 0}, {.count = 10, .value = 10}}}),
                    @"countのある最後のvalueがcount以上");
    XCTAssertThrows((number_components{false, {{.count = 0, .value = 0}, {.count = 10, .value = 0}}}),
                    @"最後のindexじゃ無いのにcountが0");
}

- (void)test_set_is_minus {
    number_components components{false, {{.count = 2}}};

    XCTAssertFalse(components.is_minus());

    components.set_is_minus(true);

    XCTAssertTrue(components.is_minus());
}

- (void)test_set_unit_value {
    number_components components{false, {{.count = 2}, {.count = 10}, {.count = 0}}};

    XCTAssertEqual(components.unit_value(0), 0);
    XCTAssertEqual(components.unit_value(1), 0);
    XCTAssertEqual(components.unit_value(2), 0);

    XCTAssertNoThrow(components.set_unit_value(1, 0));
    XCTAssertEqual(components.unit_value(0), 1);
    XCTAssertThrows(components.set_unit_value(2, 0));
    XCTAssertEqual(components.unit_value(0), 1);

    XCTAssertNoThrow(components.set_unit_value(1, 1));
    XCTAssertEqual(components.unit_value(1), 1);
    XCTAssertNoThrow(components.set_unit_value(9, 1));
    XCTAssertEqual(components.unit_value(1), 9);
    XCTAssertThrows(components.set_unit_value(10, 1));
    XCTAssertEqual(components.unit_value(1), 9);

    // 最後のcountは0なので制限なし
    XCTAssertNoThrow(components.set_unit_value(1, 2));
    XCTAssertEqual(components.unit_value(2), 1);
    XCTAssertNoThrow(components.set_unit_value(99999, 2));
    XCTAssertEqual(components.unit_value(2), 99999);

    XCTAssertThrows(components.set_unit_value(0, 3));
}

- (void)test_is_zero {
    XCTAssertTrue(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}})
            .is_zero());
    XCTAssertTrue(
        (number_components{true, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}})
            .is_zero());

    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 1}}})
            .is_zero());
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 1}, {.count = 2, .value = 0}}})
            .is_zero());
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 1}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}})
            .is_zero());
}

- (void)test_equal {
    XCTAssertTrue(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) ==
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    XCTAssertTrue(
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) ==
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    // 同じ構造のゼロ同士
    XCTAssertTrue(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) ==
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}));
    // 同じ構造のゼロならis_minusが違っていてもtrue
    XCTAssertTrue(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) ==
        (number_components{true, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}));

    // is_minusが違う
    XCTAssertFalse(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) ==
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    // valueが違う
    XCTAssertFalse(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) ==
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 1}, {.count = 100, .value = 99}}}));
    // countが違う
    XCTAssertFalse((number_components{
                       false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) ==
                   (number_components{
                       false, {{.count = 2, .value = 1}, {.count = 1000, .value = 9}, {.count = 100, .value = 99}}}));
    // ゼロ同士でcountが違う
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) ==
        (number_components{false, {{.count = 2, .value = 0}, {.count = 3, .value = 0}, {.count = 2, .value = 0}}}));
}

- (void)test_not_equal {
    XCTAssertFalse(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) !=
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    XCTAssertFalse(
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) !=
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    // 同じ構造のゼロ同士
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) !=
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}));
    // 同じ構造のゼロならis_minusが違っていてもtrue
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) !=
        (number_components{true, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}));

    // is_minusが違う
    XCTAssertTrue(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) !=
        (number_components{true, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}));
    // valueが違う
    XCTAssertTrue(
        (number_components{false,
                           {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) !=
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 1}, {.count = 100, .value = 99}}}));
    // countが違う
    XCTAssertTrue((number_components{
                      false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}) !=
                  (number_components{
                      false, {{.count = 2, .value = 1}, {.count = 1000, .value = 9}, {.count = 100, .value = 99}}}));
    // ゼロ同士でcountが違う
    XCTAssertTrue(
        (number_components{false, {{.count = 2, .value = 0}, {.count = 2, .value = 0}, {.count = 2, .value = 0}}}) !=
        (number_components{false, {{.count = 2, .value = 0}, {.count = 3, .value = 0}, {.count = 2, .value = 0}}}));
}

- (void)test_is_equal_structure {
    XCTAssertTrue(
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}
             .is_equal_structure(number_components{
                 true, {{.count = 2, .value = 0}, {.count = 10, .value = 8}, {.count = 100, .value = 98}}})));

    // unitsの数が違う
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}
             .is_equal_structure(number_components{true, {{.count = 2, .value = 0}, {.count = 10, .value = 8}}})));
    // countが違う
    XCTAssertFalse(
        (number_components{false, {{.count = 2, .value = 1}, {.count = 10, .value = 9}, {.count = 100, .value = 99}}}
             .is_equal_structure(number_components{
                 true, {{.count = 2, .value = 0}, {.count = 11, .value = 8}, {.count = 100, .value = 98}}})));
}

- (void)test_less_than {
    // 両方プラスで右の1unit目が大きい
    XCTAssertTrue((number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}} <
                   number_components{false, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}}));
    // 両方プラスで左の1unit目が大きいが、右の2unit目が大きい
    XCTAssertTrue((number_components{false, {{.count = 10, .value = 2}, {.count = 10, .value = 1}}} <
                   number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 2}}}));
    // 両方マイナスで左の1unit目が大きい
    XCTAssertTrue((number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}} <
                   number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}}));
    // 両方マイナスで右の1unit目が大きいが、左の2unit目が大きい
    XCTAssertTrue((number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 2}}} <
                   number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 1}}}));
    // 左のvalueが大きいがマイナス
    XCTAssertTrue((number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}} <
                   number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}}));
    // 右がゼロで左がマイナス
    XCTAssertTrue((number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}} <
                   number_components{false, {{.count = 10, .value = 0}, {.count = 10, .value = 0}}}));

    // ゼロ同士
    XCTAssertFalse((number_components{false, {{.count = 10, .value = 0}, {.count = 10, .value = 0}}} <
                    number_components{false, {{.count = 10, .value = 0}, {.count = 10, .value = 0}}}));

    // 両方プラスで左の1unit目が大きい
    XCTAssertFalse((number_components{false, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}} <
                    number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}}));
    // 両方プラスで右の1unit目が大きいが、左の2unit目が大きい
    XCTAssertFalse((number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 2}}} <
                    number_components{false, {{.count = 10, .value = 2}, {.count = 10, .value = 1}}}));
    // 両方マイナスで右の1unit目が大きい
    XCTAssertFalse((number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}} <
                    number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}}));
    // 両方マイナスで左の1unit目が大きいが、右の2unit目が大きい
    XCTAssertFalse((number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 1}}} <
                    number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 2}}}));
    // 右のvalueが大きいがマイナス
    XCTAssertFalse((number_components{false, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}} <
                    number_components{true, {{.count = 10, .value = 2}, {.count = 10, .value = 0}}}));
    // 左がゼロで右がマイナス
    XCTAssertFalse((number_components{false, {{.count = 10, .value = 0}, {.count = 10, .value = 0}}} <
                    number_components{true, {{.count = 10, .value = 1}, {.count = 10, .value = 0}}}));
}

- (void)test_adding {
    // ゼロ同士
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 0}, {.count = 0, .value = 0}}}.adding(
                       number_components{false, {{.count = 10, .value = 0}, {.count = 0, .value = 0}}})),
                   (number_components{false, {{.count = 10, .value = 0}, {.count = 0, .value = 0}}}));
    // 左がゼロ
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 0}, {.count = 0, .value = 0}}}.adding(
                       number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}})),
                   (number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}}));
    // 右がゼロ
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}}.adding(
                       number_components{false, {{.count = 10, .value = 0}, {.count = 0, .value = 0}}})),
                   (number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}}));
    // プラス同士
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 11}}}.adding(
                       number_components{false, {{.count = 10, .value = 2}, {.count = 0, .value = 22}}})),
                   (number_components{false, {{.count = 10, .value = 3}, {.count = 0, .value = 33}}}));
    // マイナス同士
    XCTAssertEqual((number_components{true, {{.count = 10, .value = 1}, {.count = 0, .value = 11}}}.adding(
                       number_components{true, {{.count = 10, .value = 2}, {.count = 0, .value = 22}}})),
                   (number_components{true, {{.count = 10, .value = 3}, {.count = 0, .value = 33}}}));
    // プラスが大きい
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 5}, {.count = 0, .value = 67}}}.adding(
                       number_components{true, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}})),
                   (number_components{false, {{.count = 10, .value = 4}, {.count = 0, .value = 44}}}));
    // プラスが大きい（左右入れ替え）
    XCTAssertEqual((number_components{true, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}}.adding(
                       number_components{false, {{.count = 10, .value = 5}, {.count = 0, .value = 67}}})),
                   (number_components{false, {{.count = 10, .value = 4}, {.count = 0, .value = 44}}}));
    // マイナスが大きい
    XCTAssertEqual((number_components{true, {{.count = 10, .value = 5}, {.count = 0, .value = 67}}}.adding(
                       number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}})),
                   (number_components{true, {{.count = 10, .value = 4}, {.count = 0, .value = 44}}}));
    // マイナスが大きい（左右入れ替え）
    XCTAssertEqual((number_components{false, {{.count = 10, .value = 1}, {.count = 0, .value = 23}}}.adding(
                       number_components{true, {{.count = 10, .value = 5}, {.count = 0, .value = 67}}})),
                   (number_components{true, {{.count = 10, .value = 4}, {.count = 0, .value = 44}}}));
    // プラス同士で桁を跨ぐ
    XCTAssertEqual(
        (number_components{false, {{.count = 10, .value = 9}, {.count = 20, .value = 10}, {.count = 0, .value = 100}}}
             .adding(number_components{
                 false, {{.count = 10, .value = 2}, {.count = 20, .value = 9}, {.count = 0, .value = 200}}})),
        (number_components{false, {{.count = 10, .value = 1}, {.count = 20, .value = 0}, {.count = 0, .value = 301}}}));
    // マイナス同士で桁を跨ぐ
    XCTAssertEqual(
        (number_components{true, {{.count = 10, .value = 9}, {.count = 20, .value = 10}, {.count = 0, .value = 100}}}
             .adding(number_components{
                 true, {{.count = 10, .value = 2}, {.count = 20, .value = 9}, {.count = 0, .value = 200}}})),
        (number_components{true, {{.count = 10, .value = 1}, {.count = 20, .value = 0}, {.count = 0, .value = 301}}}));
    // プラスが大きくて桁を跨ぐ
    XCTAssertEqual(
        (number_components{false, {{.count = 10, .value = 1}, {.count = 20, .value = 3}, {.count = 0, .value = 200}}}
             .adding(number_components{
                 true, {{.count = 10, .value = 2}, {.count = 20, .value = 6}, {.count = 0, .value = 100}}})),
        (number_components{false, {{.count = 10, .value = 9}, {.count = 20, .value = 16}, {.count = 0, .value = 99}}}));
    // マイナスが大きくて桁を跨ぐ
    XCTAssertEqual(
        (number_components{true, {{.count = 10, .value = 1}, {.count = 20, .value = 3}, {.count = 0, .value = 200}}}
             .adding(number_components{
                 false, {{.count = 10, .value = 2}, {.count = 20, .value = 6}, {.count = 0, .value = 100}}})),
        (number_components{true, {{.count = 10, .value = 9}, {.count = 20, .value = 16}, {.count = 0, .value = 99}}}));
}

@end
