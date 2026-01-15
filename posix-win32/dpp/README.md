# Example: DPP Console

![DPP in a console](dpp_con.png)

This example corresponds to the QM Tutorial at:

https://www.state-machine.com/qpc/tut_dpp.html

The following files comprise the project:

- the `dpp_console.qm` model file, from which all other
  files are generated:

  - `app.h`      - the generated interface for the DPP application
  - `bsp.h`      - the generated interface for the Board Support Package
  - `bsp.c`      - the generated code for the Board Support Package
  - `main.c`     - the generated code for the main function
  - `philo.c`    - the generated code for the Philo Active Object
  - `table.c`    - the generated code for the Table Active Object
  - `Makefile`   - the generated makefile to build Blinky on Windows/Linux/macOS

> NOTE: The code for the DPP example has been generated automatically
by the [QM modeling tool](https://www.state-machine.com/products/qm).
However, the code is very readable and can be created manually without any tools.

## DPP State Machines

![Philo state machine](SM_of_Philo.svg)

![Table state machine](SM_of_Table.svg)
