cmake_minimum_required(VERSION 3.6)
project(SERVANTBase)

SET(CMAKE_CXX_FLAGS " -static ${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "-static ${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

# asio library
include_directories(../servant-dependencies/asio-1.10.6/include)
# libcurl
link_directories("../servant-dependencies/curl-7.53.1/lib")
link_directories("../servant-dependencies/boost_1_63_0/lib")
include_directories("../servant-dependencies/curl-7.53.1/include/curl")
include_directories("../servant-dependencies/nlohmann-json")
include_directories("../servant-dependencies/boost_1_63_0")

include_directories(./src)

add_definitions(-DASIO_STANDALONE -DCURL_STATICLIB)

set(SOURCE_FILES
        src/VBoxWrapperClient.cpp
        src/Logger.cpp
        src/VBoxWrapperMessenger.cpp
        src/PackageManager.cpp
        src/VirtualMachine.cpp
        src/HtmlFileDownloader.cpp
        src/VMWrapperHolder.cpp
        src/FirstTimeUtilities.cpp
        src/PerformanceManager.cpp src/ProgramHolder.cpp src/ProgramHolder.hpp src/ServantBase.cpp src/WindowsUtilities.cpp src/WindowsUtilities.hpp src/Utilities.cpp src/Utilities.hpp src/ProfileManager.cpp src/ProfileManager.hpp src/ConfigManager.cpp src/ConfigManager.hpp src/MallManager.cpp src/MallManager.hpp src/MallItem.cpp src/MallItem.hpp)

add_library(SERVANTBase ${SOURCE_FILES})
target_link_libraries(SERVANTBase stdc++fs curl ws2_32 wldap32 boost_locale)