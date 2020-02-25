#ifndef FService_ERROR_H
#define FService_ERROR_H

#if (defined _WIN32)
#include <windows.h>
#elif (defined __linux)
#include <stdint.h>
#endif

#include <stdexcept>

#include <uerrno/uerrno.h>
#include <Std/Utility.h>
#include <Smuggler/BTypes/String.h>

extern "C" {

/**
 * @brief
 *  Struct to describe the error message codes of the Core.
 * @details
 */
struct _cErrorDesc {
    int  code;
    const char *message;
};

}

using Pos = size_t;
using ParseError = Pair<Pos, String>;

std::string toString(error_t err);

class NotImplemented : public std::logic_error {
public:
    NotImplemented() : std::logic_error("Function not yet implemented") { }
};

#endif
