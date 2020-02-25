/**
 * @file   NetworkService.h
 * @author Javier Jaramago Fernández(jaramago.fernandez.javier@gmail.com)
 * @date   February, 2020
 * @brief  Main file of the NetworkService for the "resourcer" service.
 * @details
 * The NetworkService is responsible of:
 *     - Accepting and parsing the requests.
 *     - Notifying clients when a request have been received, if it was accepted
 *       or declined and why.
 *     - Queueing the accepted requests for the ProcService to handle them.
 */

#ifndef MS_NETSERVICE_H
#define MS_NETSERVICE_H

/**
 * @defgroup AsioDefinitions
 * @brief Definitions for Asio networking library support in windows.
 * @details
 *  The following definitions should be placed in front of Asio and Windows
 *  inclusion headers.
 */

/** @ingroup AsioDefinitions */
#define ASIO_STANDALONE
/** @ingroup AsioDefinitions */
#define ASIO_HAS_STD_ADDRESSOF
/** @ingroup AsioDefinitions */
#define ASIO_HAS_STD_ARRAY
/** @ingroup AsioDefinitions */
#define ASIO_HAS_CSTDINT
/** @ingroup AsioDefinitions */
#define ASIO_HAS_STD_SHARED_PTR
/** @ingroup AsioDefinitions */
#define ASIO_HAS_STD_TYPE_TRAITS

// ============================ DEPENDENT =====================================
#include <boost/asio.hpp>
#if defined(_WIN32)
#include <Ole2.h>
#else
#endif
// ============================================================================

#include <Smuggler/LogServ/LogService.h>
#include <Smuggler/Payloads/Request.h>

#include <Smuggler/BTypes/String.h>

#include <Phi/Functional/Either.h>
#include <Phi/Sync/SVar.h>

#include <stdint.h>

// Naming for Unicode sequences
static std::string_view SpaceChar { "\u0020" };
static std::string_view HyphenChar { "\u002D" };
static std::string_view ColonChar { "\u003A" };

struct NetOpts {
    const String& tlsCertPath;
    const String& tlsDhCertPath;
};

/**
 * @brief
 *  Initializes the network service.
 * @param sReqQueue
 *  The queue in which the new received operations are going to be queued.
 * @param sLogQueue
 *  The queue which is going to be filled with the debugging information for the logging service.
 * @param sStopFlag
 *  Flag holded inside a SVar to advice when the service should be stopped.
 * @param netServiceThread
 *  Thread representing the network service.
 * @param port
 *  If NULL, a error is returned if port passed in first parameter is not available.
 *  If not NULL, a random port will be selected, until finding one that is available,
 *  that selected port will be returned in this parameter.
 * @return
 *  E_FN_SUCESS or one of the following errors:
 *  **TODO**
 */
errno_t startNetService(
    uint16_t                cport,
    SVar<RequestQueue>&     sReqQueue,
    SVar<LogQueue>&         sLogQueue,
    SVar<bool>&             sStopFlag,
    const NetOpts&          options,
    std::thread*            netServThread
);

#endif // !MS_NETSERVICE_H
