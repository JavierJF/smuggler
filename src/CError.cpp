#include <Smuggler/Error.h>

/* PError method to help library consumers */
struct _cErrorDesc cErrordesc[] = {
    { E_FN_SUCCESS,         "Function success" },
    { E_FN_INVAL,           "Invalid input" },
    { E_FN_INVAL_ARRSZ,     "Not enough space in array input parameter" },
    { E_FN_INVAL_INIT,      "Invalid uninitialized argument" },
    { E_FN_ABORTED,         "Aborted function execution" },
    { E_FN_NOSYS,           "Function not implemented" },
    { E_FN_NOTSUP,          "Functionality not supported" },
    { E_GV_SYS_POSIXERROR,  "Error from OS, use errno() to retrieve it" },
    { E_GV_SYS_WIN32ERROR,  "Error from OS, use GetLastError() to retrieve it" },
    { E_GV_SYS_MACOSERROR,  "Error from OS, use ... to retrieve it" }
};

std::string toString(error_t err) {
    int n = sizeof(cErrordesc)/sizeof(cErrordesc[0]);
    for (int i = 0; i < n; ++i) {
        if (err == cErrordesc[i].code) {
            return std::string(cErrordesc[i].message);
        }
    }
    return std::string("Undefined error");
}
