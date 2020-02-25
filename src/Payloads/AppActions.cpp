#include <Payloads/AppAction.h>

// AppInfo related functions
// ============================================================================
void to_json(json& j, const AppInfo& appInfo) {
    j = json { "AppInfo", {{"Name", appInfo.name}, {"Path", appInfo.path}} };
}

void from_json(const json& j, AppInfo& appInfo) {
    try {
        appInfo.name = j.at("Name").get<std::string>();
        appInfo.path = j.at("Path").get<std::string>();
    } catch (std::out_of_range& ex) {
        throw std::invalid_argument(
            std::string("-> AppInfo: ") + ex.what()
        );
    }
}

bool operator==(const AppInfo& app1, const AppInfo& app2) {
    return (app1.name == app2.name) && (app1.path == app2.path);
}
// ============================================================================

// AppAction related functions
// ============================================================================
bool operator==(const AppProcAction& action1, const AppProcAction& action2) {
    bool sameInfo = action1.info == action2.info;
    bool sameCode = action1.code == action2.code;

    return (sameInfo && sameCode);
}

std::string toString(AppActionCode code) {
    if (code == AppActionCode::AppAction_Start) {
        return "AppAction_Start";
    } else if (code == AppActionCode::AppAction_Stop) {
        return "AppAction_Stop";
    } else {
        return "AppAction_ENODEF";
    }
}

AppActionCode parseAppActionCode(std::string s) {
    if (s == "AppAction_Start") {
        return AppActionCode::AppAction_Start;
    } else if (s == "AppAction_Stop") {
        return AppActionCode::AppAction_Stop;
    } else {
        return AppActionCode::AppAction_ENODEF;
    }
}

// TODO: Check automatic conversion to String from Enums
void to_json(json& j, const AppProcAction& action) {
    j = json { {"Info", action.info}, {"Code", toString(action.code) } };
}

void from_json(const json& j, AppProcAction& action) {
    action.info = j.at("AppInfo");
    action.code = parseAppActionCode(j.at("Action").get<std::string>());
}
// ============================================================================