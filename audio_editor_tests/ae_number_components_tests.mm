//
//  ae_number_components_tests.mm
//

#import <XCTest/XCTest.h>
#include <audio_editor_core/ae_number_components.h>
#include <optional>

using namespace yas;
using namespace yas::ae;

@interface ae_number_components_tests : XCTestCase

@end

@implementation ae_number_components_tests

- (void)test_initial {
    number_components const components{false,
                                       {{.size = 2, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}}};

    std::vector<number_components_unit> const expected_units{
        {.size = 2, .value = 1}, {.size = 10, .value = 5}, {.size = 100, .value = 55}};

    XCTAssertFalse(components.is_minus());

    XCTAssertEqual(components.size(), 3);
    XCTAssertEqual(components.unit(0).value, 1);
    XCTAssertEqual(components.unit(1).value, 5);
    XCTAssertEqual(components.unit(2).value, 55);
    XCTAssertEqual(components.units(), expected_units);
}

- (void)test_constructor_throws {
    XCTAssertThrows((number_components{false, {}}), @"unitsが無い");
    XCTAssertThrows((number_components{false, {{.size = 10, .value = 10}, {.size = 10, .value = 0}}}),
                    @"valueがsize以上");
    XCTAssertThrows((number_components{false, {{.size = 10, .value = 0}, {.size = 10, .value = 10}}}),
                    @"sizeのある最後のvalueがsize以上");
    XCTAssertThrows((number_components{false, {{.size = 0, .value = 0}, {.size = 10, .value = 0}}}),
                    @"最後のindexじゃ無いのにsizeが0");
}

- (void)test_is_minus_replaced {
    {
        number_components const source{false, {{.size = 2}}};

        {
            auto const replaced = source.is_minus_replaced(true);

            XCTAssertFalse(source.is_minus());
            XCTAssertTrue(replaced.is_minus());
            XCTAssertEqual(source.units(), replaced.units());
        }

        {
            auto const replaced = source.is_minus_replaced(false);

            XCTAssertFalse(source.is_minus());
            XCTAssertFalse(replaced.is_minus());
            XCTAssertEqual(source.units(), replaced.units());
        }
    }

    {
        number_components const source{true, {{.size = 2}}};

        {
            auto const replaced = source.is_minus_replaced(true);

            XCTAssertTrue(source.is_minus());
            XCTAssertTrue(replaced.is_minus());
            XCTAssertEqual(source.units(), replaced.units());
        }

        {
            auto const replaced = source.is_minus_replaced(false);

            XCTAssertTrue(source.is_minus());
            XCTAssertFalse(replaced.is_minus());
            XCTAssertEqual(source.units(), replaced.units());
        }
    }
}

- (void)test_unit_value_replaced {
    number_components const source{false, {{.size = 2}, {.size = 10}, {.size = 0}}};

    XCTAssertEqual(source.unit(0).value, 0);
    XCTAssertEqual(source.unit(1).value, 0);
    XCTAssertEqual(source.unit(2).value, 0);

    std::optional<number_components> replaced = std::nullopt;

    XCTAssertNoThrow(replaced = source.unit_value_replaced(1, 0));
    XCTAssertTrue(replaced.has_value());
    XCTAssertEqual(replaced->unit(0).value, 1);

    replaced = std::nullopt;

    XCTAssertThrows(replaced = source.unit_value_replaced(2, 0));
    XCTAssertFalse(replaced.has_value());

    replaced = std::nullopt;

    XCTAssertNoThrow(replaced = source.unit_value_replaced(1, 1));
    XCTAssertTrue(replaced.has_value());
    XCTAssertEqual(replaced->unit(1).value, 1);

    replaced = std::nullopt;

    XCTAssertNoThrow(replaced = source.unit_value_replaced(9, 1));
    XCTAssertTrue(replaced.has_value());
    XCTAssertEqual(replaced->unit(1).value, 9);

    replaced = std::nullopt;

    XCTAssertThrows(replaced = source.unit_value_replaced(10, 1));
    XCTAssertFalse(replaced.has_value());

    replaced = std::nullopt;

    XCTAssertNoThrow(replaced = source.unit_value_replaced(1, 2));
    XCTAssertTrue(replaced.has_value());
    XCTAssertEqual(replaced->unit(2).value, 1);

    replaced = std::nullopt;

    XCTAssertNoThrow(replaced = source.unit_value_replaced(99999, 2));
    XCTAssertTrue(replaced.has_value());
    XCTAssertEqual(replaced->unit(2).value, 99999);
}

- (void)test_is_zero {
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}})
            .is_zero());
    XCTAssertTrue((number_components{true, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}})
                      .is_zero());

    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 1}}})
            .is_zero());
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 1}, {.size = 2, .value = 0}}})
            .is_zero());
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}})
            .is_zero());
}

- (void)test_equal {
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) ==
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    XCTAssertTrue(
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) ==
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    // 同じ構造のゼロ同士
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) ==
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}));
    // 同じ構造のゼロならis_minusが違っていてもtrue
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) ==
        (number_components{true, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}));

    // is_minusが違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) ==
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    // valueが違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) ==
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 1}, {.size = 100, .value = 99}}}));
    // sizeが違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) ==
        (number_components{false, {{.size = 2, .value = 1}, {.size = 1000, .value = 9}, {.size = 100, .value = 99}}}));
    // ゼロ同士でsizeが違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) ==
        (number_components{false, {{.size = 2, .value = 0}, {.size = 3, .value = 0}, {.size = 2, .value = 0}}}));
}

- (void)test_not_equal {
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) !=
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    XCTAssertFalse(
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) !=
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    // 同じ構造のゼロ同士
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) !=
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}));
    // 同じ構造のゼロならis_minusが違っていてもtrue
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) !=
        (number_components{true, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}));

    // is_minusが違う
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) !=
        (number_components{true, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}));
    // valueが違う
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) !=
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 1}, {.size = 100, .value = 99}}}));
    // sizeが違う
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}) !=
        (number_components{false, {{.size = 2, .value = 1}, {.size = 1000, .value = 9}, {.size = 100, .value = 99}}}));
    // ゼロ同士でsizeが違う
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 0}, {.size = 2, .value = 0}, {.size = 2, .value = 0}}}) !=
        (number_components{false, {{.size = 2, .value = 0}, {.size = 3, .value = 0}, {.size = 2, .value = 0}}}));
}

- (void)test_is_equal_structure {
    XCTAssertTrue(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}
             .is_equal_structure(number_components{
                 true, {{.size = 2, .value = 0}, {.size = 10, .value = 8}, {.size = 100, .value = 98}}})));

    // unitsの数が違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}
             .is_equal_structure(number_components{true, {{.size = 2, .value = 0}, {.size = 10, .value = 8}}})));
    // sizeが違う
    XCTAssertFalse(
        (number_components{false, {{.size = 2, .value = 1}, {.size = 10, .value = 9}, {.size = 100, .value = 99}}}
             .is_equal_structure(number_components{
                 true, {{.size = 2, .value = 0}, {.size = 11, .value = 8}, {.size = 100, .value = 98}}})));
}

- (void)test_less_than {
    // 両方プラスで右の1unit目が大きい
    XCTAssertTrue((number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}} <
                   number_components{false, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}}));
    // 両方プラスで左の1unit目が大きいが、右の2unit目が大きい
    XCTAssertTrue((number_components{false, {{.size = 10, .value = 2}, {.size = 10, .value = 1}}} <
                   number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 2}}}));
    // 両方マイナスで左の1unit目が大きい
    XCTAssertTrue((number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}} <
                   number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}}));
    // 両方マイナスで右の1unit目が大きいが、左の2unit目が大きい
    XCTAssertTrue((number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 2}}} <
                   number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 1}}}));
    // 左のvalueが大きいがマイナス
    XCTAssertTrue((number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}} <
                   number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}}));
    // 右がゼロで左がマイナス
    XCTAssertTrue((number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}} <
                   number_components{false, {{.size = 10, .value = 0}, {.size = 10, .value = 0}}}));

    // ゼロ同士
    XCTAssertFalse((number_components{false, {{.size = 10, .value = 0}, {.size = 10, .value = 0}}} <
                    number_components{false, {{.size = 10, .value = 0}, {.size = 10, .value = 0}}}));

    // 両方プラスで左の1unit目が大きい
    XCTAssertFalse((number_components{false, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}} <
                    number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}}));
    // 両方プラスで右の1unit目が大きいが、左の2unit目が大きい
    XCTAssertFalse((number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 2}}} <
                    number_components{false, {{.size = 10, .value = 2}, {.size = 10, .value = 1}}}));
    // 両方マイナスで右の1unit目が大きい
    XCTAssertFalse((number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}} <
                    number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}}));
    // 両方マイナスで左の1unit目が大きいが、右の2unit目が大きい
    XCTAssertFalse((number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 1}}} <
                    number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 2}}}));
    // 右のvalueが大きいがマイナス
    XCTAssertFalse((number_components{false, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}} <
                    number_components{true, {{.size = 10, .value = 2}, {.size = 10, .value = 0}}}));
    // 左がゼロで右がマイナス
    XCTAssertFalse((number_components{false, {{.size = 10, .value = 0}, {.size = 10, .value = 0}}} <
                    number_components{true, {{.size = 10, .value = 1}, {.size = 10, .value = 0}}}));
}

- (void)test_adding {
    // ゼロ同士
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 0}, {.size = 0, .value = 0}}}.adding(
                       number_components{false, {{.size = 10, .value = 0}, {.size = 0, .value = 0}}})),
                   (number_components{false, {{.size = 10, .value = 0}, {.size = 0, .value = 0}}}));
    // 左がゼロ
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 0}, {.size = 0, .value = 0}}}.adding(
                       number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}})),
                   (number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}}));
    // 右がゼロ
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}}.adding(
                       number_components{false, {{.size = 10, .value = 0}, {.size = 0, .value = 0}}})),
                   (number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}}));
    // プラス同士
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 11}}}.adding(
                       number_components{false, {{.size = 10, .value = 2}, {.size = 0, .value = 22}}})),
                   (number_components{false, {{.size = 10, .value = 3}, {.size = 0, .value = 33}}}));
    // マイナス同士
    XCTAssertEqual((number_components{true, {{.size = 10, .value = 1}, {.size = 0, .value = 11}}}.adding(
                       number_components{true, {{.size = 10, .value = 2}, {.size = 0, .value = 22}}})),
                   (number_components{true, {{.size = 10, .value = 3}, {.size = 0, .value = 33}}}));
    // プラスが大きい
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 5}, {.size = 0, .value = 67}}}.adding(
                       number_components{true, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}})),
                   (number_components{false, {{.size = 10, .value = 4}, {.size = 0, .value = 44}}}));
    // プラスが大きい（左右入れ替え）
    XCTAssertEqual((number_components{true, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}}.adding(
                       number_components{false, {{.size = 10, .value = 5}, {.size = 0, .value = 67}}})),
                   (number_components{false, {{.size = 10, .value = 4}, {.size = 0, .value = 44}}}));
    // マイナスが大きい
    XCTAssertEqual((number_components{true, {{.size = 10, .value = 5}, {.size = 0, .value = 67}}}.adding(
                       number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}})),
                   (number_components{true, {{.size = 10, .value = 4}, {.size = 0, .value = 44}}}));
    // マイナスが大きい（左右入れ替え）
    XCTAssertEqual((number_components{false, {{.size = 10, .value = 1}, {.size = 0, .value = 23}}}.adding(
                       number_components{true, {{.size = 10, .value = 5}, {.size = 0, .value = 67}}})),
                   (number_components{true, {{.size = 10, .value = 4}, {.size = 0, .value = 44}}}));
    // プラス同士で桁を跨ぐ
    XCTAssertEqual(
        (number_components{false, {{.size = 10, .value = 9}, {.size = 20, .value = 10}, {.size = 0, .value = 100}}}
             .adding(number_components{
                 false, {{.size = 10, .value = 2}, {.size = 20, .value = 9}, {.size = 0, .value = 200}}})),
        (number_components{false, {{.size = 10, .value = 1}, {.size = 20, .value = 0}, {.size = 0, .value = 301}}}));
    // マイナス同士で桁を跨ぐ
    XCTAssertEqual(
        (number_components{true, {{.size = 10, .value = 9}, {.size = 20, .value = 10}, {.size = 0, .value = 100}}}
             .adding(number_components{
                 true, {{.size = 10, .value = 2}, {.size = 20, .value = 9}, {.size = 0, .value = 200}}})),
        (number_components{true, {{.size = 10, .value = 1}, {.size = 20, .value = 0}, {.size = 0, .value = 301}}}));
    // プラスが大きくて桁を跨ぐ
    XCTAssertEqual(
        (number_components{false, {{.size = 10, .value = 1}, {.size = 20, .value = 3}, {.size = 0, .value = 200}}}
             .adding(number_components{
                 true, {{.size = 10, .value = 2}, {.size = 20, .value = 6}, {.size = 0, .value = 100}}})),
        (number_components{false, {{.size = 10, .value = 9}, {.size = 20, .value = 16}, {.size = 0, .value = 99}}}));
    // マイナスが大きくて桁を跨ぐ
    XCTAssertEqual(
        (number_components{true, {{.size = 10, .value = 1}, {.size = 20, .value = 3}, {.size = 0, .value = 200}}}
             .adding(number_components{
                 false, {{.size = 10, .value = 2}, {.size = 20, .value = 6}, {.size = 0, .value = 100}}})),
        (number_components{true, {{.size = 10, .value = 9}, {.size = 20, .value = 16}, {.size = 0, .value = 99}}}));
}

@end
