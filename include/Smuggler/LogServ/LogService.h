#ifndef MS_LOGGER_H
#define MS_LOGGER_H

#include <Phi/Sync/SVar.h>
#include <uerrno/uerrno.h>
#include <Std/Queue.h>

#include <string>
#include <thread>

template <typename T>
using Queue = std::deque<T>;
using String = std::string;
using Phi::SVar;

/**
 * @brief Typedef to improve visibility of when receiving a queue for logging. The type, Queue<String>
 *        doesn't really represent the purpose of the parameter.
 */
using LogQueue = Queue<String>;

/**
 * @brief
 *  Starts the logging service thread.
 * @param sLogQueue
 *  Logging queue with the messages to log.
 * @param sStopFlag
 *  Stop flag for stopping the thread when requested.
 * @param logServThread
 *  Thread that holds the logging service.
 * @return
 *  Error E_FN_SUCCESS if success, or one of the following errors:
 *         **TODO¨**
 */
syserr_t startLogService(SVar<LogQueue>& sLogQueue, SVar<bool>& sStopFlag, std::thread& logServThread);

#endif
