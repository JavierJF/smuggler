#include <Std/Vector.h>
#include <Std/Queue.h>
#include <Std/Utility.h>

#include <Phi/PTypes.h>
#include <Phi/Iter/While.h>
#include <Phi/Functional/Functor.h>

#include <Smuggler/BTypes/Env.h>
#include <Smuggler/Common/Date.h>
#include <Smuggler/Common/Parser.h>

namespace Smuggler {

errno_t mktime(tmz_t* time) {
    if (time == nullptr) {
        return E_FN_INVAL;
    }

    time_t  curTimeNum = 0;
    errno_t error = 0;
    tm      sTime;

    curTimeNum = std::time(nullptr);

    if (error == E_FN_SUCCESS) {
#if defined(_WIN32)
        error = localtime_s(&sTime, &curTimeNum);
#else
        error = localtime_r(&curTimeNum, &sTime) == nullptr;
#endif

        if (error == E_FN_SUCCESS) {
            long timezone = 0;
#if defined(_WIN32)
            error = _get_timezone(&timezone);
#else
            timezone = ::timezone;
#endif

            if (error == E_FN_SUCCESS) {
                tm* tmSubSt = reinterpret_cast<tm*>(time);
                *tmSubSt = sTime;
                time->tm_gmtoff = timezone;
            } else {
                time = nullptr;
            }
        }
    } else {
        time = nullptr;
        error = E_FN_INVAL;
    }

    return error;
}

const String pZeroAndSign(const int num, const bool sign) {
    String result;

    if (sign) {
        const String baseNum = std::to_string(num);

        if (num < 10 && num > 0) {
            result = "+0" + baseNum;
        } else if (num >= 10) {
            result = "+" + baseNum;
        } else if (num > -10 && num < 0) {
            result = baseNum.substr(0, 1) + "0" + baseNum.substr(1);
        } else if (num <= -10) {
            result = baseNum;
        }
    } else {
        int absNum = std::abs(num);
        const String baseNum = std::to_string(absNum);

        if (num < 10 && num >= 0) {
            result = "0" + baseNum;
        } else if (num >= 10) {
            result = baseNum;
        }
    }

    return result;
}

const String serialize(const tmz_t& time) {
    constexpr int secsHour= 3600;
    constexpr int baseYear = 1900;
    constexpr int monthOffset = 1;

    String year = std::to_string(time.tm_year + baseYear);
    String month = pZeroAndSign(time.tm_mon + monthOffset, false);
    String day = pZeroAndSign(time.tm_mday, false);

    String hour = pZeroAndSign(time.tm_hour, false);
    String min = pZeroAndSign(time.tm_min, false);
    String sec = pZeroAndSign(time.tm_sec, false);

    String ymd = year + HyphenChar + month + HyphenChar + day;
    String hms = "T" + hour + ColonChar + min + ColonChar + sec;

    double offDHM = time.tm_gmtoff / secsHour;
    double offDHours = 0;
    double offDMins  = 0;

    offDMins = std::modf(offDHM, &offDHours);
    long int offHours = static_cast<long int>(std::round(offDHours));
    long int offMins = static_cast<long int>(std::round(offDMins * 60));

    String sOffHours = pZeroAndSign(offHours, true);
    String sOffMins = pZeroAndSign(offMins, false);

    String tz = sOffHours + ColonChar + sOffMins;

    return ymd + hms + tz;
}

Either<ParseError, tmz_t> deserialize(const String& time) {
    Deque<Deque<T>> delimiters {
         { T { HyphenChar, 2, 5 } },
         { T { u8"T",      1, 3 } },
         { T { ColonChar,  2, 3 } },
         { T { HyphenChar, 1, 3 }, T { u8"+", 1, 3 } },
         { T { ColonChar,  1, 3 } }
    };

    auto eRes = tokenize(time, delimiters);

    if (isLeft(eRes)) {
        return fromLeft(eRes);
    } else {
        auto timeValues = fromRight(eRes);

        if (timeValues.size() != 8) {
            return ParseError { -1, "ASSERT: tmz_t parsing result shouldn't be partial at this point." };
        }

        auto year  = timeValues[0].first;
        auto month = timeValues[1].first;
        auto day   = timeValues[2].first;
        auto hour  = timeValues[3].first;
        auto min   = timeValues[4].first;
        auto sec   = timeValues[5].first;

        auto timeValSymbol = timeValues[5].second;

        auto h_gtm_off  = std::atoi(timeValues[6].first.c_str());
        auto min_gtm_off  = std::atoi(timeValues[7].first.c_str());

        auto toff = (h_gtm_off * 3600) + (min_gtm_off * 60);

        if (timeValSymbol == u8"-") {
            toff = -toff;
        }

        tmz_t result {
            std::atoi(sec.c_str()),
            std::atoi(min.c_str()),
            std::atoi(hour.c_str()),
            std::atoi(day.c_str()),
            std::atoi(month.c_str()) - 1,
            std::atoi(year.c_str()) - 1900,
            0,
            0,
            0,
            toff
        };

        errno_t epoch_time = Smuggler::mktime(&result);
        if (epoch_time == -1) {
            return ParseError { -1, "Error: mktime failed, invalid time as input." };
        } else {
            return result;
        }
    }
}

}
