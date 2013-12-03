moxa_api
========

Set of functions for controlling a SerialDongle adapter for Wireless
Sensor Network data gathering. 

This code is designed to run into a Moxa W315A embedded computer. It
receives and parses data from serial port in a specific protocol defined 
to this purpose.

Although this API need a Moxa cross compiler, the flag USE_GCC could be
enable in order to test the code with a PC. A python program was added
to generate random data and simulates the device behavior. 
