#include <Std/Vector.h>
#include <Std/Queue.h>
#include <Std/Utility.h>

#include <Phi/PTypes.h>
#include <Phi/Iter/While.h>

#include <Smuggler/Common/Date.h>
#include <Smuggler/Common/Parser.h>

#include <gtest/gtest.h>

using namespace Smuggler;

TEST(Date, tmzMktime) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    ASSERT_EQ(res, E_FN_SUCCESS);
}

TEST(Date, serializeTmz) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    if (res != E_FN_SUCCESS) {
        FAIL() << ("SerializeTmz: mktime failed with error: " + std::to_string(res));
    }
    const String sTime = serialize(time);
}

TEST(Date, deserializeTmzCorrect) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    if (res != E_FN_SUCCESS) {
        FAIL() << ("SerializeTmz: mktime failed with error: " + std::to_string(res));
    }
    const String sTime = "2001-10-09T1:53:11-4:30";
    auto eTime = deserialize(sTime);

    pMatch(eTime,
        [](auto& pError) {
            FAIL() << ("SerializeTmz: mktime failed with error: " + pError.second);
        },
        [](auto&) {
            SUCCEED();
        }
    );
}

TEST(Date, deserializeTmzWrong) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    if (res != E_FN_SUCCESS) {
        FAIL() << ("SerializeTmz: mktime failed with error: " + std::to_string(res));
    }
    const String sTime = "-09T1:53:11-4:30";
    auto eTime = deserialize(sTime);

    pMatch(eTime,
        [](auto&) {
            SUCCEED();
        },
        [](auto&) {
            FAIL() << ("SerializeTmz: Wrong date, should fail");
        }
    );
}

std::string random_string( size_t length ) {
    auto randchar = []() -> char {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "?¿:-ç^*Ç}{[]";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ static_cast<size_t>( rand() ) % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

TEST(Date, deserializeTmzRandom) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    if (res != E_FN_SUCCESS) {
        FAIL() << ("SerializeTmz: mktime failed with error: " + std::to_string(res));
    }

    for (size_t i = 0; i < 40; i++) {
        const String sTime = random_string(i);
        auto eTime = deserialize(sTime);

        pMatch(eTime,
            [](auto&) {},
            [](auto&) {
                FAIL() << ("SerializeTmz: Random date, should fail");
            }
        );
    }

    SUCCEED();
}

TEST(Date, deserializeTmz) {
    tmz_t time;
    auto res = Smuggler::mktime(&time);
    if (res != E_FN_SUCCESS) {
        FAIL() << ("SerializeTmz: mktime failed with error: " + std::to_string(res));
    }
    const String sTime = serialize(time);
    auto eTime = deserialize(sTime);

    pMatch(eTime,
        [](auto& pError) {
            FAIL() << ("SerializeTmz: mktime failed with error: " + pError.second);
        },
        [&sTime](auto& time) {
            auto nSTime = serialize(time);
            ASSERT_EQ(nSTime, sTime);
        }
    );
}
