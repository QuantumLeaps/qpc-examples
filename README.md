# About qpc-examples
This `qpc-examples` repository contains the examples for the QP/C Real-Time Embedded Framework.
The following examples are enhanced with `cmake` support:
- [Posix/Win32 dpp example](https://github.com/stefanschober/qpc-examples/tree/cmakeSupport/posix-win32/dpp)
- [QWin GUI dpp example](https://github.com/stefanschober/qpc-examples/tree/cmakeSupport/qwin-gui/dpp-gui)

This [Posix/Win32 cmake example](https://github.com/stefanschober/qpc-examples/tree/cmakeSupport/posix-win32-cmake/dpp) is a full featured example, which shows how to use `cmake`with qpc to automatically generate build systems for different platforms and configurations.

Another [example](https://github.com/stefanschober/TrafficLight), a simple trafficlight simulation, shows how to set up a system with various BSPs for different target systems and use `cmake` to build them with the matching cross compilers from one combined source code repository.

The setting of the variable `QPC_SDK_PATH`, which points to the `qpc` directory inside the example's `CMakeLists.txt` file assumes, that the `examples` directory is part of the `qpc` tree and resides in `<...>/qpc/examples`.

> **NOTE**
This repository is **NOT** designed to be used standalone, but rather only as submodule of the repository [qpc](https://github.com/QuantumLeaps/qpc)