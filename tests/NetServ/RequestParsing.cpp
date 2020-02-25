#include <gtest/gtest.h>
#include <Smuggler/NetServ/RequestParsing.h>
#include <Smuggler/BTypes/String.h>

#include <variant>
#include <iostream>
#include <string>
#include <type_traits>
#include <iomanip>
#include <Phi/Functional/Either.h>

// TEST(RequestParsing, RemoveSubStrOccurrences) {
//     std::string str = "Hello \n\nNothing \n";
//     std::string substr = "\n";
//
//     replace(str, substr);
//
//     std::string res = "Hello Nothing ";
//
//     ASSERT_EQ(res, str);
// }
//
// template<class T> struct always_false : std::false_type {};
//
// TEST(RequestParsing, ParsingAppConfigJsonPayload) {
//     // a JSON text
//     std::string text = R"(
//     {
//         "Apps": [
//             {
//                 "AppInfo": {
//                     "Name": "Notepad.exe",
//                     "Path": "C:\\System32\\notepad.exe"
//                 },
//                 "Settings": [
//                     {
//                         "Name": "Font Size",
//                         "ExMode": "Direct",
//                         "UIMode": {
//                             "Name": "Edit",
//                             "Id": "Edit"
//                         },
//                         "Actions": [
//                             {
//                                 "ElemId": "Format",
//                                 "Action": "Expand"
//                             },
//                             {
//                                 "ElemId": "Accept",
//                                 "Action": "Invoke"
//                             }
//                         ]
//                     }
//                 ]
//             }
//         ]
//     }
//     )";
//
//     auto pRes = parseRequest(text);
//
//     pMatch(pRes,
//         [](auto& requests) {
//             for (auto request : requests) {
//                 std::visit(
//                     [](auto&& arg) -> void {
//                         using T = std::decay_t<decltype(arg)>;
//                         if constexpr (std::is_same_v<T,Vector<AppUISettings>>) {
//                             std::cout << std::setw(4) << json { arg[0] } << '\n';
//                         } else if constexpr (std::is_same_v<T, AppProcAction>) {
//                             FAIL() << "Incorrect parsing result";
//                         } else if constexpr (std::is_same_v<T, ServiceAction>) {
//                             FAIL() << "Incorrect parsing result";
//                         } else {
//                             static_assert(always_false<T>::value, "non-exhaustive visitor!");
//                         }
//                     },
//                     request
//                 );
//             }
//
//             ASSERT_FALSE(requests.empty());
//         },
//         [](auto err){
//             FAIL() << err.msg;
//         }
//     );
// }

static std::string HyphenChar { "\u002D" };

TEST(StringFunctions, SplitString) {
    String sep { "t" };
    String str { "jfatñtlkjattñlajtadsft" };
    Vector<String> strVec {"jfa", "ñ", "lkja", "", "ñlaj", "adsf", ""};

    auto res = split(str, sep);

    ASSERT_EQ(strVec, res);
}

TEST(StringFunctions, SplitUnicode) {
    String str {u8"ハロー・ワールド"};
    String sep {u8"ロ"};
    Vector<String> strVec { u8"ハ", u8"ー・ワールド" };

    auto res = split(str, sep);
    ASSERT_EQ(strVec, res);
}

struct MoveExample {
    uint8_t s = 0;
    uint8_t c = 0;

    MoveExample(uint8_t s) { this->s = s; }
    MoveExample(const MoveExample& ex) {
        ADD_FAILURE_AT("MoveExample: Copy constructor", 164);
        this->s = ex.s;
    }
    MoveExample(MoveExample&& other) {
        this->s = other.s + 1;
    }
    MoveExample& operator=(const MoveExample&) {
        ADD_FAILURE_AT("MoveExample: Copy assignment operator", 171);
        return *this;
    }
    MoveExample& operator=(MoveExample&& other) {
        this->c = other.c + 1;
        return *this;
    }
};

TEST(Either, movingCalls) {
    // First move
    Either<MoveExample, bool> e { MoveExample {1} };

    // Second move as input Either { std::move(e) }
    Either<MoveExample, bool> res = pMatch( std::move(e),
        [](MoveExample&& m) {
            // Third move when parameter passing to function.
            EXPECT_EQ(m.s, 2);
            // Fourth move when parameter passing to function.
            return std::move( m );
        },
        [](bool&& val) {
            ADD_FAILURE_AT("errorMoving: Not valid value", 192);
            return val;
        }
    );

    // Fifth move when passing res, and six move when returning the value.
    auto m = std::move( fromLeft( std::move( res ) ) );

    EXPECT_EQ(m.s, 6);
}
