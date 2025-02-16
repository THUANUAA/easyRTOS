cd build\Debug
cmake -G Ninja ../..
ninja
pyocd flash -t STM32F103C8 easyRTOS.elf
cd ..\..