#include <Smuggler/NetServ/RequestParsing.h>
#include <Smuggler/BTypes/Vector.h>
#include <Smuggler/BTypes/String.h>

#include <rapidjson/document.h>
#include <rapidjson/error/error.h>
#include <rapidjson/error/en.h>

#include <Phi/Functional/Either.h>
#include <Phi/Functional/Functor.h>
#include <Phi/Iter/While.h>
#include <boost/lexical_cast.hpp>

#include <codecvt>
#include <algorithm>

using namespace Phi;

namespace json = rapidjson;

String jsonParsingErrMsg(json::ParseResult parseRes) {
    if (parseRes == true) {
        return "RequestParsing: {}";
    } else {
        String errMsgHeader = "RequestParsing: { ";
        String arrayStart { "{ " };
        String offsetField = "Offset: " + std::to_string(parseRes.Offset());
        String fieldSep { ", " };
        String codeField = String(json::GetParseError_En(parseRes.Code())) ;
        String arrayEnd { " }" };

        String errMsg =
                errMsgHeader +
                arrayStart +
                offsetField +
                fieldSep +
                codeField +
                arrayEnd;

        return errMsg;
    }
}

struct DateParseState {
    String dateInfo;
    DateData parsedData;
};

Either<Date, ParseError> parseDate(std::string) {
    return Date {};
}

struct LCastError {};

Either<uint32_t, LCastError> parseNumber(String str) {
    try {
        return boost::lexical_cast<uint32_t>(str);
    } catch(...) {
        return LCastError {};
    }
}

using ParseRes = Either<uint32_t, LCastError>;

Either<Vector<uint32_t>, Error> parseNumbers(Vector<String> sNumbers) {
    Vector<uint32_t> result;

    for (auto sNumber : sNumbers) {
        auto parseRes = parseNumber(sNumber);

        if (isRight(parseRes)) {
            return Error { E_FN_INVAL, "parseNumbers: Failed to parse number - " + sNumber };
        } else {
            result.push_back( fromLeft(parseRes) );
        }
    }

    return move(result);
}

Vector<String> split(const String& str, String sp) {
    Vector<String> res {};
    size_t actualPos = 0;
    size_t nextPos = 0;

    while( actualPos <= str.length() ) {
        nextPos = str.find( sp, actualPos );

        if (nextPos != String::npos) {
            res.push_back(str.substr(actualPos, nextPos - actualPos));
            actualPos = nextPos + sp.length();
        } else {
            res.push_back( str.substr(actualPos) );
            break;
        }
    }

    return res;
}

bool basePayloadStructureCheck(json::Document payload) {
    // bool res = payload.HasMember("User");
    // res = res || payload.HasMember("User");
    // res = res || payload.HasMember("Timestamp");

    // return res;
    return false;
}

bool newPasswordPayload(json::Document) {
    return false;
}

String verifyInputFormat(json::Document payload) {
    // bool hasUser = payload.HasMember("User");
    // bool hasPassword = payload.HasMember("User");
    // bool hasAction = payload.HasMember("Action");

    // payload.HasMember("User");

    return "";
}

Either<ParseError, Vector<Request>> parseRequest(String strRequest) {
    json::Document requestJSON;
    json::ParseResult parseRes = requestJSON.Parse(strRequest.c_str());

    if (!parseRes) {}
    return ParseError {0, std::string {"Not Implemented."} };
}
