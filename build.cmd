@ECHO OFF
mkdir build
cd build
cmake -G "NMake Makefiles" ..
nmake