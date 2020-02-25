// Networking
#define ASIO_STANDALONE
#define ASIO_HAS_STD_ADDRESSOF
#define ASIO_HAS_STD_ARRAY
#define ASIO_HAS_CSTDINT
#define ASIO_HAS_STD_SHARED_PTR
#define ASIO_HAS_STD_TYPE_TRAITS

// Networking
#include <boost/asio.hpp>
// After including asio we need to specifically import interface defs.
#if defined(_WIN32)
#include <Ole2.h>
#else
#endif

#include <atomic>
#include <thread>

// Operations
#include <utility>
#include <algorithm>
#include <queue>

// Utilities
#include <string>
#include <stdint.h>

// For user info
#include <iostream>

// Logging (own)
#include <Smuggler/LogServ/LogService.h>

// Networking (own)
#include <Smuggler/NetServ/NetworkService.h>

// Concurrency (own)
#include <Phi/Sync/SVar.h>
#include <Phi/Sync/WMutex.h>
#include <Std/CV.h>

using namespace Phi;

// Utilities
#include <Std/Vector.h>

int main(int argc, char* argv[]) {
    // Stop flag for cleanly exiting all service threads.
    // ======================================================================
    SVar<bool> sStopFlag { false };

    // Start LogService
    // ======================================================================
    SVar<LogQueue>  sLogQueue { LogQueue {} };
    std::thread     logServThread;

    //  In the future a interface for different loggers will be provided.
    //  std::list<logInterfaces> loggers;

    startLogService(sLogQueue, sStopFlag, logServThread);
    // ======================================================================

    // Start NetService
    // ======================================================================
    uint16_t           assignedPort = 0;
    std::thread        netServThread;
    SVar<RequestQueue> sReqQueue { RequestQueue{} };
    // SRVar<AppUIElemActionQueue> sUIElemActionQueue { uiServiceSyncPs, uiElemActionFlag, appUIElemActionQueue };

    // TLS certificates path
    NetOpts netOpts = {
        "",
        ""
    };

    startNetService(
        31400,
        sReqQueue,
        sLogQueue,
        sStopFlag,
        netOpts,
        &netServThread
    );

    auto errMsg = std::string("NetService: Assigned port is ") + std::to_string(assignedPort);

    // updSVar(sLogQueue, [&errMsg](auto queue) { queue.push(errMsg); });

    // Main thread should have a mechanism to update services and to ensure that
    // they are all working and restart them in case of internal, unexpected failure.
    while (!readSVar(sStopFlag)) {
    }

    // Wait for all services threads to finish before exiting main thread.
    // ======================================================================
    logServThread.join();
    // uiServThread.join();
    // netServThread.join();

    return 0;
}
