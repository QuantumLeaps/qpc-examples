## Brought to you by:
[![Quantum Leaps](https://www.state-machine.com/attachments/logo_ql_400.png)](https://www.state-machine.com)
<hr>

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/QuantumLeaps/qpc-examples)](https://github.com/QuantumLeaps/qpc-examples/releases/latest)


# About qpc-examples

This repository contains examples for the QP/C Real-Time Embedded Framework
(see https://github.com/QuantumLeaps/qpc )

This repository can be used in two ways:

## Standalone (examples in the `cmsis-packs` folder)

```
+---cmsis-packs    <=== standalone examples
|   +---dpp_nucleo-c031c6-qk-gcc
|   +---dpp_nucleo-c031c6-qk-keil
|   \---dpp_nucleo-c031c6-qk-stm32cube
|
```

## As a sub-module to the [qpc](https://github.com/QuantumLeaps/qpc) repository

```
+---arm-cm
|   +---blinky_ek-tm4c123gxl
|   +---blinky_nucleo-c031c6
|   +---dpp_efm32-slstk3401a
|   |   +---qk
|   |   |   +---armclang
|   |   |   +---gnu
|   |   |   \---iar
|   |   +---qv
|   |   |   +---armclang
|   |   |   +---gnu
|   |   |   \---iar
|   |   +---qview
|   |   |   \---img
|   |   \---qxk
|   |       +---armclang
|   |       +---gnu
|   |       \---iar
|   +---dpp_ek-tm4c123gxl
|   +---dpp_ek-tm4c123gxl_mpu
|   +---dpp_mbed-lpc1768
|   +---dpp_nucleo-c031c6
|   +---dpp_nucleo-c031c6_mpu
|   +---dpp_nucleo-h743zi
|   +---dpp_nucleo-l053r8
|   +---dpp_nucleo-l152re
|   +---dpp_nucleo-l552ze
|   +---dpp_stm32f4-discovery
|   +---game_efm32-slstk3401a
|   +---low-power_ek-tm4c123gxl
|   \---real-time_nucleo-l053r8
|
+---arm-cr
|   +---blinky_launchxl2-tms57012
|   \---dpp_launchxl2-tms57012
|
+---embos
|   \---arm-cm
|       \---dpp_nucleo-h743zi
+---emwin
|   +---demo_no_wm
|   \---demo_with_wm
+---freertos
|   \---arm-cm
|       +---dpp_ek-tm4c123gxl
|       \---dpp_nucleo-h743zi
+---lwip
|   \---arm-cm
|       \---lwip_ek-lm3s6965
+---msp430
|   +---blinky_msp-exp430f5529lp
|   +---blinky_msp-exp430g2
|   \---dpp_msp-exp430f5529lp
|
+---pic32
|   \---dpp_microstick2-pic32
|
+---posix-win32
|   +---blinky
|   +---calc
|   +---calc2
|   +---calc_sub
|   +---comp
|   +---defer
|   +---dpp
|   +---dpp-posix
|   +---dpp_comp
|   +---history_qhsm
|   +---history_qmsm
|   +---qhsmtst
|   +---qmsmtst
|   +---reminder
|   \---reminder2
|
+---qutest
|   +---blinky
|   +---dpp
|   +---dpp-comp
|   +---evt_par
|   +---qhsmtst
|   +---qmsmtst
|   +---self_test
|   +---start_seq
|   +---unity_basic
|   +---unity_ledbar
|   +---unity_ledbar2
|   +---unity_mock
|   \---unity_strlen
|       +---qutest
|       +---src
|       \---test
|
+---qwin-gui
|   +---dpp-gui
|   \---game-gui
|
+---threadx
|   \---arm-cm
|       +---dpp_ek-tm4c123gxl
|       \---dpp_stm32f429-discovery
+---uc-os2
|   \---arm-cm
|       \---dpp_ek-tm4c123gxl
\---zephyr
    +---blinky
    \---dpp
```
