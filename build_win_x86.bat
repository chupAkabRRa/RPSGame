if not exist .\bin\win\x86 mkdir .\bin\win\x86
if not exist .\build\win\x86 mkdir .\build\win\x86
cd build\win\x86
cmake -G "Visual Studio 14 2015" ..\..\..
cmake --build . --config Release --target install
cd ..\..\..