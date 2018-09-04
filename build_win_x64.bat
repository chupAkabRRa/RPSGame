if not exist .\bin\win\x64 mkdir .\bin\win\x64
if not exist .\build\win\x64 mkdir .\build\win\x64
cd build\win\x64
cmake -G "Visual Studio 14 2015 Win64" ..\..\..
cmake --build . --config Release --target install
cd ..\..\..