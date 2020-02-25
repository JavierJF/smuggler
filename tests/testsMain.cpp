#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <gtest/gtest.h>

#if defined(_WIN32) && defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

#if defined(_WIN32) && defined(_DEBUG)
    // Get a checkpoint of the memory after Google Test has been initialized.
    _CrtMemState memoryState = {0};
    _CrtMemCheckpoint( &memoryState );
   // Send all reports to STDOUT
   _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
   _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
   //Check for memleaks
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    RUN_ALL_TESTS();
    // Check for leaks after tests have run
    _CrtMemDumpAllObjectsSince( &memoryState );
    // Show a more complete memory dump.
    // _CrtDumpMemoryLeaks();

    return 0;
}
