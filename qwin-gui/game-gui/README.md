# About this Example
This directory contains the "Fly 'n' Shoot" game example application
ultimately intended for the EFM32-SLSTK3401A board (Pearl Gecko
evaluation kit) board. However, this is a pixel-exact emulation of the
game based on the QWin GUI Prototyping Toolkit, see:

- https://www.state-machine.com/products/qtools#QWIN

# Win32-GUI Emulation
The directory `qpc\examples\qwin-gui\game-gui` contains the emulation
of the "Fly 'n' Shoot" game on Windows GUI, either single-threaded (win32-qv)
or multithreaded (win32). This sub-directory contains the Visual Studio
solution file (game-gui.sln) for Microsoft Visual C++.

<p align="center">
<img src="qwin_ani.gif"/><br>
<b>"Fly 'n' Shoot" game emulation running on Windows</b>
</p>

> __NOTE:__ The emulation uses exactly the same code as the embedded board
(directory `qpc\examples\arm-cm\game_efm32-slstk3401a`) and differs
only in the Board Support Package (bsp.c). This example demonstrates the
"dual targeting" development approach, where most of the embedded code is
developed on the workstation (Windows), but is intended for a deeply
embedded target (EFM32-SLSTK3401A here).

## QS Software Tracing Instrumentation
The "win32-gui" emulation also supports the "Spy" build configuration,
in which case it attempts to connect to the QSPY host application via
a TCP/IP socket (default). Here is how you launch the QSPY host utility
(assuming that QTools have been installed and the `qtools/bin` directory
is in the PATH):

```
qspy
```
