#include <Smuggler/LogServ/LogService.h>

#include <iostream>
#include <functional>

void _logServiceTh(SVar<LogQueue>& sLogQueue, SVar<bool>& sStopFlag) {
    while(!readSVar(sStopFlag)) {
        LogQueue logQueue = takeSVar(sLogQueue);

        while(!logQueue.empty()) {
            std::string msg = logQueue.front();
            std::cout << msg << "\r\n";
            logQueue.pop_back();
        }
    }
}

syserr_t startLogService(
    SVar<LogQueue>&     , // sLogQueue,
    SVar<bool>&         , // sStopFlag,
    std::thread&         // logServThread
) {
    syserr_t error = E_FN_SUCCESS;

    // TODO: Identify possible error codes and doc them.
    try {
        // logServThread =
        //     std::thread(
        //         _logServiceTh,
        //         CRef(sLogQueue),
        //         CRef(sStopFlag)
        //     );
    } catch (std::system_error err) {
        return error = err.code().value();
    }

    return error;
}
