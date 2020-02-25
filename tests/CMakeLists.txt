IF(CMAKE_BUILD_TESTS)
    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})

    ADD_EXECUTABLE(Tests
       ${PROJECT_SOURCE_DIR}/tests/NetServ/Date.cpp
       ${PROJECT_SOURCE_DIR}/tests/NetServ/NetService.cpp
       ${PROJECT_SOURCE_DIR}/tests/NetServ/RequestParsing.cpp
    )
    TARGET_COMPILE_DEFINITIONS(Tests
       PRIVATE
       PWD="${CMAKE_SOURCE_DIR}"
    )
    TARGET_LINK_LIBRARIES(Tests
        ${GTEST_BOTH_LIBRARIES}
        smuggler
        ${CMAKE_DL_LIBS}
        ${CONAN_LIBS}
    )
   IF(ENABLE_COVERAGE)
      SETUP_TARGET_FOR_COVERAGE(coverageHtml tests coverage)
   ENDIF()

   ADD_TEST(NAME GTESTS COMMAND Tests)
ENDIF()
