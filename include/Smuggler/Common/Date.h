#ifndef FLAGGER_DATE_H
#define FLAGGER_DATE_H

#include <Smuggler/BTypes/String.h>
#include <Smuggler/Error.h>

#include <Phi/PTypes.h>
#include <Phi/Functional/Either.h>

#include <uerrno/uerrno.h>

#include <ctime>

using namespace Phi;

namespace Smuggler {

static String SpaceChar { "\u0020" };
static String ColonChar { "\u003A" };
static String PlusChar  { "\u002B" };
static String HyphenChar { "-" };

#if (defined _BSD_SOURCE)

#else

struct tmz_t {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
    int tm_isdst;
    // GNU Extensions
    long int tm_gmtoff;
};

namespace tmz {

enum PropertyNum {
    tm_sec        = 0,
    tm_min        = 1,
    tm_hour       = 2,
    tm_mday       = 3,
    tm_mon        = 4,
    tm_year       = 5,
    tm_wday       = 6,
    tm_yday       = 7,
    tm_isdst      = 8,
    tm_gmtoff     = 9
};

int& accessProperty(tmz::PropertyNum, tmz_t date);

}

/**
 * @brief mktime
 * @param time
 * @return If the operation was a success returns E_FN_SUCCESS, if not one of the
 *  following errors:
 *     - E_FN_INVAL if the paramter is NULL.
 *     - Unspecified errno value in case of Windows _get_timezone fails.
 */
errno_t mktime(tmz_t* time);

/**
 * Serializes a PInt8 to a TimeZone string from a ISO 8601 date format.
 *
 * @brief serializeTZ
 * @param tz
 * @return
 */
const String serialize(const tmz_t& tz);

/**
 * @brief deserialize
 * @param time
 * @return
 */
Either<ParseError, tmz_t> deserialize(const String& time);

#endif

}

#endif
