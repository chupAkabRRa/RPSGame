cd build\win\x64
cmake -G "Visual Studio 14 2015 Win64" ..\..\..
cmake --build . --config Release --target install
cd ..\..\..