About this Example
==================
This example demonstrates how to use the IAR EWARM IDE to build
a QP application.


IAR Project File
----------------
The IAR EWARM project file provided with this example uses relative paths
to the QP framework location (includes, port, and libraries. These
relative paths must be modified when the project is moved to different
relative location.


Stack Size and Heap Size
------------------------
In this project, the size of the C stack and heap are determined in
the linker script (.icf file, see the next section).


Linker Script
-------------
The IAR linker script provides a template of the recommended linker script
for QP applications. This file needs to be customized to set the
application-specific sizes of the Stack and Heap. This file can be edited
from the IAR EWARM IDE via the Project Options/Linker settings.
