#SERVANT Base Library
SERVANT is a assistant program to let you install and manage virtualized Linux server on Windows easily.   
You can get servers from SERVANT's online services, download, install and run it in SERVANT Manager. Servers are packaged as a virtual machine.  
SERVANT uses Virtualbox for VM management.  

##Dependencies
executable of servant-windows-vboxwrapper(at runtime) and working Virtualbox(5.0 or later)  
CMake 3.6 or later  
MinGW-w64 and g++ that have experimental/filesystem support.  
asio-1.10.6(without boost)  
libcurl-7.53.1
nlohmann/json 2.1.1  

##Build
Put asio,nlohmann/json and libcurl in this relative path, or just edit CMakeLists.txt:  
../servant-dependencies/asio-1.10.6/  
../servant-dependencies/curl-7.53.1/  
../servant-dependencies/nlohmann-json/  

##Note
Only the code that launch executable of vboxwrapper(servant-windows-vboxwrapper) is Windows-dependent.  
This project is still under heavy development.  
