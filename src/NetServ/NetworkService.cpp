#include <boost/asio/ssl.hpp>

#include <Smuggler/NetServ/NetworkService.h>
#include <Smuggler/NetServ/RequestParsing.h>
#include <Smuggler/LogServ/LogService.h>
#include <Smuggler/Payloads/Request.h>
#include <Smuggler/Payloads/ServiceAction.h>

#include <Phi/Sync/SVar.h>
#include <Phi/Sync/MSVar.h>

#include <Phi/Iter/While.h>
#include <Phi/Functional/ADT.h>
#include <Phi/Functional/Function.h>

#include <Std/Utility.h>
#include <Std/Functional.h>

#include <string>
#include <thread>
#include <atomic>

using Socket = boost::asio::ip::tcp::socket;
template <class T>
using Fn = std::function<T>;
using Phi::Either;
using Phi::While;
using Phi::SVar;

Either<boost::system::error_code, std::string> readline(Socket& socket) {
    boost::asio::streambuf buf;
    boost::system::error_code error;
    boost::asio::read_until(socket, buf, "\x03", error);

    if (!error) {
        auto rawData = boost::asio::buffer_cast<const char*>(buf.data());
        std::string data;

        if (*rawData != '\0') {
            data = boost::asio::buffer_cast<const char*>(buf.data());
        } else {
            data = "";
        }

        // This is just in case delimiter is unwanted from the String.
        data.erase( --data.end() );

        return move(data);
    } else {
        auto msg = error.message();
        return error;
    }
}

boost::system::error_code sendline(Socket& socket, const std::string& str ) {
    const std::string msg = str + "\n";
    try {
        boost::asio::write( socket, boost::asio::buffer(msg) );
    } catch (boost::system::system_error& ex) {
        return ex.code();
    }

    return boost::system::error_code();
}

bool isErrorOrShutdown(Either<boost::system::error_code, ServiceActionCode> msg) {
    auto res =
        pMatch(
            msg,
            [] (auto err) {
                if (err == boost::asio::error::eof) {
                    return true;
                } else if (err == boost::asio::error::broken_pipe) {
                    return true;
                } else if (err == boost::asio::error::connection_aborted) {
                    return true;
                } else if (err == boost::asio::error::connection_aborted) {
                    return true;
                } else if (err == boost::asio::error::interrupted) {
                    return true;
                } else {
                    // If unknown error, no shutdown.
                    return false;
                }
            },
            [] (auto action) {
                if (action == ServiceActionCode::ServAction_Stop) {
                    return true;
                } else {
                    return false;
                }
            }
       );

    return res;
}

/**
 * @brief
 *  Process new request.
 * @param socket
 *  Socket to be used for receiving new package.
 * @param pSync
 *  Synchronization primitives for informing when new packages arrives
 *
 * @return An error if the handling has went wrong, or the new received Request.
 */
auto procNewRequest(
    SVar<RequestQueue>&      requestQueue,
    SVar<LogQueue>&          sLogQueuye,
    std::shared_ptr<Socket>& sock
) -> Either<boost::system::error_code, ServiceActionCode>
{
    using ProcRes = Either<boost::system::error_code, ServiceActionCode>;

    auto& socket = (*sock);
    auto rawMsg = readline(socket);

    ProcRes res =
        pMatch(rawMsg,
            [](auto err) {
                return ProcRes { std::move(err) };
            },
            [&socket,&requestQueue](auto str) {
                auto request = parseRequest(str);

                ProcRes sAction =
                    pMatch(request,
                        [&socket](auto&& err) -> ProcRes {
                            std::cout << "Sending Error response." << "\r\n";
                            std::string errorMsg =
                                "Error: Invalid payload received, parsing failed"
                                "at pos [" + std::to_string(err.first) + "]"
                                "with message '" + err.second + "'";

                            sendline(socket, errorMsg);

                            return ProcRes(ServiceActionCode::ServAction_KeepAlive);
                       },
                       [&socket,&requestQueue](auto&& newRequests) -> ProcRes {
                            updSVar(
                                requestQueue,
                                [newRequests = move(newRequests)](auto& queue) {
                                    for (auto request : newRequests) {
                                        queue.push(request);
                                    }
                                }
                            );

                            std::cout << "Sending Okay response." << "\r\n";
                            sendline(socket, "Received new valid AppSettings");

                            return ProcRes(ServiceActionCode::ServAction_KeepAlive);
                        }
                    );

                return sAction;
            }
        );

    return res;
}

template <typename T>
using SharedPtr = std::shared_ptr<T>;
using std::function;

/**
 * @brief
 *  Receives operations payloads, and notify to the main thread a new
 *  operation is pending.
 */
void tcpReceiver(
    const boost::system::error_code&    err,
    std::shared_ptr<Socket>             socket,
    boost::asio::ip::tcp::acceptor&     acceptor,
    SVar<RequestQueue>&                 sReqQueue,
    SVar<LogQueue>&                     sLogQueue,
    SVar<bool>&                         sStopFlag
) {
    using ProcRes = Either<boost::system::error_code, ServiceActionCode>;

    auto nativeHandle = (*socket).native_handle();
    updSVar(
        sLogQueue,
        [nativeHandle] (std::deque<std::string>& logQueue) {
            logQueue.push_back(
                std::string("New connection stablished in socket: ") + std::to_string(nativeHandle) + "\r\n"
            );
        }
    );

    auto res =
        WhileA(
            ServiceActionCode::ServAction_KeepAlive,
            Fn<bool(ProcRes)>(isErrorOrShutdown),
            Fn<ProcRes(SVar<RequestQueue>&, SVar<LogQueue>&, SharedPtr<Socket>&)>(procNewRequest),
            sReqQueue,
            sLogQueue,
            socket
        );

    std::cout << "Closing socket: " << nativeHandle << "\r\n";
    (*socket).shutdown(boost::asio::socket_base::shutdown_both);
    (*socket).close();

    std::shared_ptr<Socket> sock(new Socket(acceptor.get_io_service()));

    std::function <void(const boost::system::error_code&)> acceptHandler =
        std::bind(
            tcpReceiver,
            std::placeholders::_1,
            sock,
            CRef(acceptor),
            CRef(sReqQueue),
            CRef(sLogQueue),
            CRef(sStopFlag)
        );

    acceptor.async_accept(
        *sock,
        acceptHandler
    );
}

using boost::asio::basic_socket_acceptor;
using boost::asio::ip::basic_endpoint;
using boost::asio::ip::tcp;
using boost::system::error_code;
using boost::asio::io_service;

Either<error_code, basic_socket_acceptor<tcp>> createTCPAcceptor(
    io_service& io_service,
    const basic_endpoint<tcp>& endpoint
) {
     try {
         boost::asio::ip::tcp::acceptor acceptor(
             io_service,
             endpoint,
             true
         );

         return move( acceptor );
     } catch (boost::system::system_error& e) {
         boost::system::error_code code = e.code();

         return code;
     }
}

errno_t startNetService(
    uint16_t            port,
    SVar<RequestQueue>& sReqQueue,
    SVar<LogQueue>&     sLogQueue,
    SVar<bool>&         sStopFlag,
    const NetOpts&      options,
    std::thread*        netServThread
) {
    errno_t resCode { E_FN_SUCCESS };

    // Thread: NetService own thread, continuosly accepting new connections for
    // requests.
    // ========================================================================
    std::thread _netServiceThread(
        [port, &sReqQueue, &sLogQueue, &sStopFlag, &resCode, &options] () {
            const String& tlsCertPath = options.tlsCertPath;
            const String& tlsDhCertPath = options.tlsDhCertPath;

            // Try to init to create a io_service and assign and endpoint to it.
            // =================================================================
            boost::asio::io_service io_service;
            boost::system::error_code acceptorErr;
            boost::asio::ssl::context context { boost::asio::ssl::context::tlsv12_server };

            context.load_verify_file(tlsCertPath);
            context.set_options(
                boost::asio::ssl::context::default_workarounds
                | boost::asio::ssl::context::no_sslv2
                | boost::asio::ssl::context::single_dh_use
            );

            auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
            auto eAcceptor = createTCPAcceptor(io_service, endpoint);

            pMatch(eAcceptor,
                [&resCode] (boost::system::error_code error) {
                    resCode = static_cast<fnerr_t>(error.value());
                },
                [&io_service, &sReqQueue, &sLogQueue, &sStopFlag] (auto& acceptor) {
                    std::shared_ptr<Socket> sock(new Socket(acceptor.get_io_service()));

                    std::function <void(const boost::system::error_code&)> acceptHandler =
                        std::bind(
                            tcpReceiver,
                            std::placeholders::_1,
                            sock,
                            CRef(acceptor),
                            CRef(sReqQueue),
                            CRef(sLogQueue),
                            CRef(sStopFlag)
                        );

                    acceptor.async_accept(
                        *sock,
                        acceptHandler
                    );

                    io_service.post([&sStopFlag, &io_service] () {
                        std::thread waiter([&sStopFlag, &io_service] () {
                            while (!readSVar(sStopFlag));
                            io_service.stop();
                        });
                        waiter.detach();
                    });

                    io_service.run();
                }
            );
    });

    // Communicate back the thread for latter handling.
    // ========================================================================
    *netServThread = std::move(_netServiceThread);

    return resCode;
}
