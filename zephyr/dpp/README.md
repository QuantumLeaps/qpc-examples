# DPP example for Zephyr

<p align="center">
<img src="qp-zephyr.webp"/>
</p>


# Overview
Dining Philosophers Problem (DPP) example with multiple
active objects.

# Building and Running (Linux)
Open a terminal in the directory of this example (`qpc/examples/zephry/dpp`).

Depending on your Zephyr installation, you might need to shell the scripts
to configure Zephyr environment:

```
source ~/zephyrproject/.venv/bin/activate
source ~/zephyrproject/zephyr/zephyr-env.sh
```

To build the example, type:
```
west build -b <board>
```
where `<board>` is one of the
[boards supported by Zephyr](https://docs.zephyrproject.org/latest/boards/index.html).
The `dpp` example has been tested with the following boards:

```
nucleo_f103rb
nucleo_l152re
nucleo_h743zi
```

## QP/Spy Build Configuration
The [QP/Spy software tracing](https://www.state-machine.com/qtools/qpspy.html)
is supported in this example and can be enabled by providing the command-line
option `-DQSPY=ON` to the build process:

Specific example
```
west build -b nucleo_h743zi -- -DQSPY=ON
```

> NOTE: The QP/Spy software tracing uses the Zephyr's console UART.
This means that the Zephyr `printk()` facility cannot be used while
QP/Spy is configured.


# Flashing the Board
To flush the board, type:
```
west flash
```

# Sample Output
The application also produces ASCII output to the serial terminal
(if suppoted by the board):

```
*** Booting Zephyr OS build v4.2.0-280-g9219c81b66f4 ***
Philo[0]->thinking
Philo[1]->thinking
Philo[2]->thinking
Philo[3]->thinking
Philo[4]->thinking
QF_onStartup
Philo[2]->hungry
Philo[2]->eating
Philo[4]->hungry
Philo[4]->eating
Philo[1]->hungry
Philo[0]->hungry
Philo[3]->hungry
```
