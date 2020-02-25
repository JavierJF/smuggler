#ifndef MS_REQUEST_H
#define MS_REQUEST_H

#include <Smuggler/Payloads/AppAction.h>
#include <Smuggler/Payloads/ServiceAction.h>
#include <Std/Vector.h>

#include <list>
#include <cassert>
#include <variant>
#include <queue>

template <typename T>
using List = std::list<T>;

/**
 * @brief Enum with the types of operations that can be requested to Smuggler service.
 */
enum RequestType {
    Req_SetAppSettings      = 0,
    Req_WatchAppSettings    = 1,
    Req_AppProcAction       = 2,
    Req_CreateUserEvent     = 3,
    Req_ServiceAction       = 4
};

/**
 * @brief
 *   Struct for holding a new requested set of operations. This operations could be:
 */
using Request =
    std::variant<
        AppProcAction,
        ServiceAction
    >;

/*
 * Removing handmade safe variant in favour of C++17 variant type
 * ============================================================================
 *
 * struct Request {
 * public:
 *     union {
 *         List<AppUISettings> appUISettings;
 *         AppAction           appAction;
 *         ServiceAction       servAction;
 *     };

 *     RequestType type;

 *     ~Request() {
 *         if (this->type == Req_SetAppSettings) {
 *             this->appUISettings.~List<AppUISettings>();
 *         } else if (this->type == Req_AppProcAction) {
 *             this->appAction.~AppAction();
 *         } else if (this->type == Req_ServiceAction) {
 *             this->servAction.~ServiceAction();
 *         }
 *     }

 *     Request(const std::list<AppUISettings>& appUISettings) {
 *         this->type = Req_SetAppSettings;
 *         new (&this->appUISettings) std::list<AppUISettings>(appUISettings);
 *     }

 *     Request(std::list<AppUISettings>&& appUISettings) {
 *         this->type = Req_SetAppSettings;
 *         new (&this->appUISettings) std::list<AppUISettings>(std::move(appUISettings));
 *     }

 *     Request(AppProcAction&& appAction) {
 *         this->type = Req_AppProcAction;
 *         new (&this->appUISettings) AppAction(std::move(appAction));
 *     }

 *     Request(ServiceAction&& servAction) {
 *         this->type = Req_ServiceAction;
 *         new (&this->appUISettings) ServiceAction(std::move(servAction));
 *     }

 *     Request(const Request& other) {
 *         this->type = other.type;

 *         if (other.type == Req_SetAppSettings) {
 *             new (&this->appUISettings) std::list<AppUISettings>(other.appUISettings);
 *         } else if (other.type == Req_AppProcAction) {
 *             new (&this->appAction) AppAction(other.appAction);
 *         } else if (other.type == Req_ServiceAction) {
 *             new (&this->appAction) AppAction(other.appAction);
 *         } else {
 *             assert(false && "Not Implemented");
 *         }
 *     }
 * };
 */

using RequestQueue = std::queue<Request>;

#endif
