#ifndef MCS_REQUESTPARSING_H
#define MCS_REQUESTPARSING_H

#include <Smuggler/Payloads/Request.h>
#include <Smuggler/Error.h>
#include <Smuggler/BTypes/String.h>

#include <Phi/Functional/Either.h>
#include <Std/Vector.h>

using Phi::Either;

using DateData = std::vector<int64_t>;

struct Error {
    error_t code;
    String msg;
};

struct Time {
    uint32_t hour;
    uint32_t min;
    uint32_t sec;
    uint32_t msec;
};

struct Date {
    uint32_t day;
    uint32_t month;
    uint32_t year;
};

struct Timestamp {
    Date date;
    Time time;
};

Vector<String> split(const String& str, String sp);
Either<ParseError, Vector<Request>> parseRequest(std::string requests);

#endif
