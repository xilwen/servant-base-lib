cmake_minimum_required(VERSION 3.6)
project(tmpSERVANTBase)
#project(testExe)

SET(CMAKE_CXX_FLAGS " -static ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "-static ${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

# asio library
include_directories("../servant-dependencies/asio-1.10.6/include")
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
        src/PerformanceManager.cpp)

add_library(tmpSERVANTBase ${SOURCE_FILES})
target_link_libraries(tmpSERVANTBase stdc++fs curl ws2_32 wldap32)

#making test undependent
#add_executable(testExe test.cpp)
#target_link_libraries(testExe tmpSERVANTBase)