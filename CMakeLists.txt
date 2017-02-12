cmake_minimum_required(VERSION 3.6)
project(SERVANTBase)
#project(testExe)

SET(CMAKE_CXX_FLAGS " -static ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "-static ${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

# asio library
include_directories(../servant-dependencies/asio-1.10.6/include)
# libcurl
link_directories("../servant-dependencies/curl-7.52.1/lib")
include_directories("../servant-dependencies/curl-7.52.1/include/curl")
include_directories(./src)

add_definitions(-DASIO_STANDALONE -DCURL_STATICLIB)

set(SOURCE_FILES
        src/vBoxWrapperClient.cpp
        src/logger.cpp
        src/vBoxWrapperMessenger.cpp
        src/packageManager.cpp
        src/VirtualMachine.cpp
        src/htmlFileDownloader.cpp
        src/vBoxWrapperHolder.cpp
        src/firstTimeUtilities.cpp
        src/PerformanceManager.cpp src/programHolderForWindows.cpp src/programHolderForWindows.hpp src/programHolder.cpp src/programHolder.hpp)

add_library(SERVANTBase ${SOURCE_FILES})
target_link_libraries(SERVANTBase stdc++fs curl ws2_32 wldap32)