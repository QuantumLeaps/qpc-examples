# About this Example
This directory contains the "Dining Philosophers Problem" (DPP) example
for uC/OS-II application running on the EK-TM4C123GXL board, a.k.a. TivaC
LauchPad (ARM Cortex-M4F). This directory contains portable code that
should compile with any C compiler for ARM Cortex-M.

<p align="center">
<img src="logo_ucos2.webp"/>
</p>

<p align="center">
<img src="ek-tm4c123gxl.webp"/><br>
<b>EK-TM4C123GXL (TivaC LaunchPad)</b>
</p>

The sub-directories contain code and project files, which are specific to
the particular ARM toolchains, such as ARM (MDK-ARM) and IAR.

Please refer to the README files in the sub-directories for specific
instructions how to use and customize the example to your needs.


# Support Code for EK-TM4C123GXL Board
The directory qpc\3rd_party\ek-tm4c123gxl contains the CMSIS-compliant
device code for the TM4C123GH6PM MCU (ARM Cortex-M4F). Please see the
README.txt file in this folder for more details.


# QS Software Tracing Instrumentation
This example provides the "Spy" build configuration, which outputs the QS
(Quantum Spy) software tracing data through UART0, which is connected to
the virtual COM port of the TI Stellaris debugger.

The output is generated at 115200 baud rate.

Here is an example invocation of the QSPY host application to receive
the QS data from EK-TM4C123GXL:

```
qspy -cCOM1
```

The actual COM port number might be different on your Windows machine.
Please check the Device Manager to find the COM port number.
