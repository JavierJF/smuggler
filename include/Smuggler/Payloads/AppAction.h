#ifndef MS_APPACTION_H
#define MS_APPACTION_H

#include <string>

/**
 * @brief
 *  Different actions that can be performed over one application.
 */
enum AppActionCode {
    AppAction_Start,
    AppAction_Stop,
    AppAction_ENODEF
};

/**
 * @brief
 *  Returns a human readable representation for the AppAction codes.
 * @param code
 *  The AppActionCode to be translated.
 * @return
 *  The string representation of 'code' param.
 */
std::string toString(AppActionCode code);

/**
 * @brief
 *  Struct with the necessary info to trigger an action over an application.
 * @var code
 *  The AppActionCode that is requested to be executed over the application.
 * @var info
 *  The AppInfo holding the information to identify the application.
 */
struct AppProcAction {
    AppActionCode   code;
};

#endif
