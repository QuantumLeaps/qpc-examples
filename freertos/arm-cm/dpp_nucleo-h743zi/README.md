> **NOTE**
This file is best viewed in a **markdown viewer**, such as the one built into GitHub. Markdown viewers are also available as plug-ins to popular Internet browsers.

# DPP on NUCLEO-H743ZI
This example demonstrates QP port to the FreeRTOS on the STM32 NUCLEO-H743ZI board (ARM Cortex-M7).

<p align="center">
<img src="logo_freertos.webp"/>
</p>

The example uses the [Dining Philosophers Problem (DPP) application](https://www.state-machine.com/qpc/tut_dpp.html)

<p align="center">
<img src="./stm32-nucleo-h743zi.webp"/><br>
<b>STM32 NUCLEO-H743ZI</b>
</p>

## Features Demonstrated
- QP port to FreeRTOS
  + with GNU-ARM toolchain
  + with Keil-MDK toolchain
  + with IAR-ARM toolchain
- multiple cooperating active objects
- immutable (const) events
- mutable (dynamic) events
- time events
- direct event posting
- publish-subscribe event delivery


## Build Configurations
- Debug
- Release
- Spy - software tracing with the built-in virtual COM port

# Code Organization
```
examples\freertos\dpp_nucleo-h743zi
|
|   +---armcland    // Keil-MDK toolchain
|   |   \---targetConfigs
|   |       dpp.uvprojx       // Keil uVision project
|   +---gnu        // GNU-ARM toolchain
|   |   \---targetConfigs
|   |       Makefile          // Makefile for GNU-ARM
|   \---iar        // IAR EWARM
|           dpp-qk.eww     // IAR EW-ARM workspace
```

# Building the example

### GNU/ARM
- open terminal window
- change to the desired directory (either `examples\freertos\dpp_nucleo-h743zi\qk\gnu`, `examples\freertos\dpp_nucleo-h743zi\qv\gnu`, or `examples\freertos\dpp_nucleo-h743zi\qxk\gnu`)
- to build the default Debug configuration, type:

```
make
```

> **NOTE**
The `make` utility for Windows is provided in the QTools collection for Windows.

- to build the Release configuration, type:

```
make CONF=rel
```

- to build the Spy configuration, type:

```
make CONF=spy
```

### ARM/KEIL MDK
- Open the provided KEIL uVision project (either `dpp-qk.uvprojx`, `dpp-qv.uvprojx`, or `dpp-qxk.uvprojx`)
in Keil uVision IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Target" drop-down menu.


### IAR EWARM
- Open the provided IAR EWARM workspace (either `dpp-qk.eww`, `dpp-qv.eww`, or `dpp-qxk.eww`)
in IAR EWARM IDE. Build/Debug/Download to the board from the IDE.
- Change the build configuration in the "Project Configuration" drop-down menu.


# Uploading the Binary to the Board
The STM32 NUCLEO boards enumerate as a USB drive when connected to the host computer. The boards then can be programmed by **copying** the binary to that USB drive. This can be useful for the command-line GNU/ARM build. For example, to program the binary produced for the Debug configuration, you can type:

```
copy dbg\dpp-qk.bin E:
```
NOTE: The above command assumes that the NUCLEO board enumerated as drive E:. Of course you need to adjust the command for your specific drive letter.

Alternatively, if you use IDEs, such as KEIL-MDK or IAR EWARM, you can program the board from the IDE (e.g., by starting a debug session).


# Tracing with QP/Spy
When the board is flashed with the Spy build configuration, it produces the QP/Spy software tracing output to the built-in virtual COM port of the TivaC LauchPad board. The trace is binary rather than ASCII, and therefore requires a special host-based application called QSPY.

> **NOTE** QSPY host application is available in the QTools collection.

To launch the QSPY host application:
- open terminal window
- type:

```
qspy -c COM5
```

where "COM5" is an example virtual COM port enumerated by the board. You need to check the specific COM port number on your host computer using the Device Manager application, Ports (COM and LPT) section.


The following screen shot shows a typical output from QSPY:

<p align="center">
<img src="./qspy-output.png"/><br>
<b>Typical QSPY output produced by the Spy build configuration</b>
</p>

