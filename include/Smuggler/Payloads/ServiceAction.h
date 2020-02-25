#ifndef MS_SERVICEACTION_H
#define MS_SERVICEACTION_H

/**
 * @brief Different actions that can be executed over a service.
 */
enum class ServiceActionCode {
    ServAction_Stop      = 0,
    ServAction_Start     = 1,
    ServAction_Restart   = 2,
    ServAction_KeepAlive = 3
};

/**
 * @brief Different services that can receive a ServiceAction.
 */
enum ServiceCode {
    ServLogging     = 0,
    ServNetwork     = 1,
    ServUIHandler   = 2,
    ServAll         = 3
};

/**
 * @brief Action to be executed over a service.
 */
struct ServiceAction {
    ServiceActionCode   actionCode;
    ServiceCode         serviceCode;
};

#endif
