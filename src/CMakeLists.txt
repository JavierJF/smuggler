INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

ADD_LIBRARY(smuggler
    ${PROJECT_SOURCE_DIR}/include/Smuggler/LogServ/LogService.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/NetServ/NetworkService.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/NetServ/RequestParsing.h

    ${PROJECT_SOURCE_DIR}/include/Smuggler/Common/Date.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/Common/Generic.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/Common/Parser.h

    ${PROJECT_SOURCE_DIR}/include/Smuggler/Payloads/AppAction.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/Payloads/Request.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/Payloads/ServiceAction.h

    ${PROJECT_SOURCE_DIR}/include/Smuggler/BTypes/String.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/BTypes/Vector.h
    ${PROJECT_SOURCE_DIR}/include/Smuggler/BTypes/Env.h

    ${PROJECT_SOURCE_DIR}/include/Smuggler/Error.h

    ${PROJECT_SOURCE_DIR}/src/LogServ/LogService.cpp
    ${PROJECT_SOURCE_DIR}/src/NetServ/NetworkService.cpp
    ${PROJECT_SOURCE_DIR}/src/NetServ/RequestParsing.cpp

    ${PROJECT_SOURCE_DIR}/src/Common/Date.cpp
    ${PROJECT_SOURCE_DIR}/src/Common/Parser.cpp

    ${PROJECT_SOURCE_DIR}/src/CError.cpp
)

TARGET_INCLUDE_DIRECTORIES(smuggler PUBLIC ${PROJECT_SOURCE_DIR}/include)

Install(TARGETS smuggler
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        CONFIGURATIONS Debug Release)

Install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include
        CONFIGURATIONS Debug Release
        PATTERN "*.modulemap" EXCLUDE
        PATTERN "*.txt" EXCLUDE)
