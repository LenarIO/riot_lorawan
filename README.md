# LoRaWAN in RIOT-OS
Brief example on how to use LoRaWAN on [RIOT-OS](http://www.riot-os.org)

# Running this example
This example can be ran in any RIOT-OS board with LoRaWAN support (embedded or provided via an external radio). By default, it will build for `ST B-L072Z-LRWAN1 LoRa discovery` (b-l072z-lrwan1 on RIOT-OS)
* Register a LoRaWAN application in [The Things Networks](http://www.thethingsnetwork.org)
* Add Cayenne LPP as an Integration, [as seen here](https://www.thethingsnetwork.org/docs/applications/cayenne/)
* Register a OTAA device to your current LoRaWAN applcation
* Fill DEV_EUI, APP_KEY and APP_EUI in `main.c` file
* Clone RIOT-OS: `git clone https://github.com/RIOT-OS/RIOT.git`
* Build and flash with `RIOTBASE=<path/to/cloned/RIOT-OS> make all flash`. If another board is used, pass BOARD parameter or modify the `Makefile` (check here to see how these variables are declared)
* Connect to the board via serial COM with `RIOTBASE=<path/to/cloned/RIOT-OS> make term`. Check the logs and ensure that the board is able to join the LoRaWAN network.
* Play around with TTN and Cayenne LPP dashboards!

Happy coding! Enjoy.

# About RIOT-OS
RIOT is a real-time multi-threading operating system that supports a range of devices that are typically found in the Internet of Things (IoT): 8-bit, 16-bit and 32-bit microcontrollers.

RIOT is based on the following design principles: energy-efficiency, real-time capabilities, small memory footprint, modularity, and uniform API access, independent of the underlying hardware (this API offers partial POSIX compliance).

RIOT is developed by an international open source community which is independent of specific vendors (e.g. similarly to the Linux community). RIOT is licensed with LGPLv2.1, a copyleft license which fosters indirect business models around the free open-source software platform provided by RIOT, e.g. it is possible to link closed-source code with the LGPL code.

Feel free to visit RIOT-OS website at [riot-os.org](http://www.riot-os.org)
