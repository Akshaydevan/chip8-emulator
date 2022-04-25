# Chip 8 Emulator 
A basic chip 8 emulator written in c++ 
#
#
## How to run
Give to path to the rom you want to run as the argument to the binary file
```
  ./chip8-emulator.exe path_to_rom
```
#
#
## BUILD
This project uses vcpkg to install dependencies
```
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE = path_to_your_vcpkg_root/scripts/buildsystems/vcpkg.cmake
make
```