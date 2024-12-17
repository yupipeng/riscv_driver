# SDK STRUCTURE

Doc document file
Example list few simple code to use this sdk

application user code should be written here, only care about how the project organized

Core initialize different peripheral by board design, initialize essential code for communication with other part
    may be generator by program in future

optek/Common riscv needed file
optek/Drivers hardware driver, direct access to the register, provide HAL to use these hardware
    chip about code
optek/Include public header file for user use
optek/Library third part library or hardware-independent code which can be compiled at any platform
optek/PrivateInclude internal usage header file, not recommend to use by user
optek/Source device driver, or something that support the system to perform different action

Test code snippet to test different functionality