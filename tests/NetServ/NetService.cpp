#include <Smuggler/NetServ/NetworkService.h>

#include <gtest/gtest.h>

TEST(NetworkService, StartStopService) {
    SVar<bool>          sStopFlag     { false };
    SVar<LogQueue>      sLogQueue     { LogQueue {} };
    SVar<RequestQueue>  sReqQueue     { RequestQueue {} };
    NetOpts             options       { PWD"/tests/Data/vault_tca.pem", "" };
    uint16_t            port          { 31400 };
    std::thread         netServThread {};

    auto netStartRes =
        startNetService(
            port,
            sReqQueue,
            sLogQueue,
            sStopFlag,
            options,
            &netServThread
        );

    ASSERT_EQ(netStartRes, E_FN_SUCCESS);

    putSVar(sStopFlag, true);
    netServThread.join();
}
