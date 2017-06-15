# SERVANT Base Library
SERVANT is a assistant program to let you install and manage virtualized Linux server on Windows easily.   
You can get servers from SERVANT's online services, download, install and run it in SERVANT Manager. Servers are packaged as a virtual machine.  
SERVANT uses Virtualbox for VM management.  

## Build Dependencies
* CMake 3.6 or later(or use IDEs like CLion)  
* MinGW-w64 and g++ that have experimental/filesystem and Windows API header support.  
* asio-1.10.6(without boost)  
* libcurl-7.53.1
* nlohmann/json 2.1.1  
* boost 1.63.0

## How to build
Put asio,nlohmann/json and libcurl in this relative path, or just edit CMakeLists.txt:  
* ../servant-dependencies/asio-1.10.6/  
* ../servant-dependencies/curl-7.53.1/  
* ../servant-dependencies/nlohmann-json/  
* ../servant-dependencies/boost_1_63_0/

## Note
* Only boost::locale is used for string conversion.   No other boost component is needed by this project.
* WindowsUtilities, VBoxWrapperHolder and ProgramHolder are Windows-dependent, while other classes should be platform-independent.
