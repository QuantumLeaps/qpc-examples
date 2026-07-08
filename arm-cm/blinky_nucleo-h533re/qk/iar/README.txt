# Issue
The IAR EWARM version used in this project (9.32.1) does not provide native support for the STM32H533RE device.

> Note: Newer versions of IAR EWARM include support for recent ST devices. Once upgraded, set the project Target device explicitly to STM32H533RE.

## Workaround
As a temporary workaround, the project Target configuration is set generically to:

- Core: Cortex-M33
- No specific STM32 device is selected.

Even with this generic target setting, the project is correctly configured for STM32H533RE through the following elements:

- The linker script "project.icf" matches the STM32H533RE memory map.
- The correct device support files and startup code are included under the project group "nucleo-h533re".
- The build defines the device macro STM32H533xx.

With these components in place, all build configurations produce valid binary images:

- dbg/project.bin
- rel/project.bin
- spy/project.bin


## Programming the NUCLEO-H533RE
Any of the generated .bin files can be programmed onto the NUCLEO-H533RE board by copying the file to the board's USB mass-storage interface.


## Debugging
Because STM32H533RE is not recognized by IAR EWARM 9.32.1, the IAR debugger cannot be used with the NUCLEO-H533RE in this configuration.
