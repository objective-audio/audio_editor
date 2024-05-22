//
//  ae_keyboard_utils.cpp
//

#include "ae_keyboard_utils.h"

#include <cpp-utils/stl_utils.h>

using namespace yas;
using namespace yas::ae;

std::optional<key> keyboard_utils::to_key(ui::key_event const &event,
                                          std::unordered_set<ae::modifier> const &modifiers) {
    static std::unordered_set<ae::modifier> const shift_modifiers{ae::modifier::shift};
    static std::unordered_set<ae::modifier> const alternate_modifiers{ae::modifier::alternate};
    static std::unordered_set<ae::modifier> const command_modifiers{ae::modifier::command};
    static std::unordered_set<ae::modifier> const function_and_numeric_modifiers{ae::modifier::function,
                                                                                 ae::modifier::numeric_pad};
    static std::unordered_set<ae::modifier> const function_and_numeric_and_shift_modifiers{
        ae::modifier::function, ae::modifier::numeric_pad, ae::modifier::shift};

    auto const &chara = event.characters;
    auto const lower_chara = to_lower(chara);
    auto const key_code = event.key_code;

    bool const empty_modifiers = modifiers.size() == 0;

    if (empty_modifiers) {
        if (key_code == 51) {
            return key::del;
        } else if (key_code == 53) {
            return key::esc;
        } else if (key_code == 36) {
            return key::ret;
        } else if (key_code == 48) {
            return key::tab;
        } else if (chara == " ") {
            return key::space;
        } else if (chara == "+") {
            return key::plus;
        } else if (chara == "-") {
            return key::hyphen;
        } else if (lower_chara == "a") {
            return key::a;
        } else if (lower_chara == "s") {
            return key::s;
        } else if (lower_chara == "d") {
            return key::d;
        } else if (lower_chara == "f") {
            return key::f;
        } else if (lower_chara == "n") {
            return key::n;
        } else if (lower_chara == "t") {
            return key::t;
        } else if (chara == "0") {
            return key::num_0;
        } else if (chara == "1") {
            return key::num_1;
        } else if (chara == "2") {
            return key::num_2;
        } else if (chara == "3") {
            return key::num_3;
        } else if (chara == "4") {
            return key::num_4;
        } else if (chara == "5") {
            return key::num_5;
        } else if (chara == "6") {
            return key::num_6;
        } else if (chara == "7") {
            return key::num_7;
        } else if (chara == "8") {
            return key::num_8;
        } else if (chara == "9") {
            return key::num_9;
        } else if (chara == "[") {
            return key::left_bracket;
        } else if (chara == "]") {
            return key::right_bracket;
        }
    }

    if (modifiers == shift_modifiers) {
        if (key_code == 48) {
            return key::shift_tab;
        } else if (lower_chara == "n") {
            return key::shift_n;
        } else if (chara == "+") {
            return key::plus;
        } else if (chara == "-") {
            return key::hyphen;
        }
    }

    if (modifiers == function_and_numeric_modifiers) {
        if (key_code == 123) {
            return key::left;
        } else if (key_code == 124) {
            return key::right;
        } else if (key_code == 125) {
            return key::down;
        } else if (key_code == 126) {
            return key::up;
        }
    }

    if (modifiers == function_and_numeric_and_shift_modifiers) {
        if (key_code == 123) {
            return key::shift_left;
        } else if (key_code == 124) {
            return key::shift_right;
        } else if (key_code == 125) {
            return key::shift_down;
        } else if (key_code == 126) {
            return key::shift_up;
        }
    }

    return std::nullopt;
}
