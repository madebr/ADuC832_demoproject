# ADuC832 demoproject

##Aims:

* Use all of the possibilities of the ADuC832 microcontroller
* Use the developer board, provider by [De Nayer](http://iiw.kuleuven.be/denayer)
    + Course website: <http://emsys.denayer.wenk.be/emedu/ADuC800/index.php>
* Use SDCC - Small Device C Compiler as free GNU compiler

##License

This project is licensed under GLPv2 as free software. You are free to use, modify and redistribute following the terms in LICENSE.

##What does work:

* UART:
    + Asynchronous (uses buffer)
    + Blocks when buffer is full
    + Configured at 9600 bauds, no parity
    + Simple printf implementation
    + Console interface
    + Configurable at compile by setting BAUDRATE
* LCD:
    + GDM1602
    + 4-bit mode
    + Write only (RW signal is connected to GND)
* ADC:
    + Only channel 7 has been tested
* RTC:
    + Timer Interval Counter
    + Interrupts at intervals
* Watchdog:
    + Resets the device if not reset frequently
* GPIO:
    + Read buttons/DIP switch

##What does not work yet/TODO:
* PWM:
    + TODO
* LCD:
    + Occasionally: does not initialize correctly (timing error?)
* ADC:
    + Continuous/DMA mode sampling
    + Temperature sensor
    + DAC
* CPU:
    + Powerdown
    + External Interrupts
    + Change clock
* Timers:
    + TODO
* System:
    + Use XRAM and Flash/EE Data Memory
    + Improve configurability
    + Decrease memory/stack footprint:
        - Use assembly for specific operations (like char to decimal)
        - Put static values in the correct data section
    + Library
    + Tests

**Please open an issue or pull request if you have a problem or want to contribute a patch.**

##Build dependencies:

- SDCC - Small Device C Compiler: <http://sdcc.sourceforge.net/>
- CMake: <https://cmake.org/>
- Git: <https://git-scm.com/>

##Build instructions:

1. Download and install the [build dependencies](##Build dependencies).
    + Mac and Windows: Download and install
    + Linux: use packet manager (apt-get or dnf)
2. Clone this repository.
3. Run CMake
4. Build
5. Program the microcontroller with the generated hex file
6. Iterate

###Commands for Linux:
```
sudo dnf install cmake sdcc git
cd /tmp
srcdir=/tmp/ADuC832_demoproject
git clone "https://github.com/madebr/ADuC832_demoproject.git" "$srcdir"
mkdir builddir && cd builddir
cmake -DCMAKE_TOOLCHAIN_FILE="$srcdir/sdcc_toolchain.cmake" "$srcdir"
make -j3
```
Fedora users should insert `-DSDCC_PREFIX="sdcc-"` in the cmake command (see `tools/cmake_fedora.sh`).

