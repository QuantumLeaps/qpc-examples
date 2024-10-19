This version of IAR does not support STM32U545RETQ.
Therefore, the selected device is: STM32U575RG and
the linker script dpp*.icf is edited to add the second
RAM region (IRAM2). Additionally, the linker script
is changed to place CSTACK at the start of IRAM2. 