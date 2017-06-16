copilot
=======

.. note::
    This is very early in the development stages. As such, many features and
    functionalities are broken or simply not yet implemented. Use at your own
    risk.

.. note::
    HERE BE DRAGONS. I DO NOT ASSUME ANY RESPONSIBILITY FOR ANYTHING YOU MAY
    BREAK WITH YOUR CAR. YOUR CAR IS A MISSILE ON WHEELS. TREAT YOUR CAR, THIS
    CODE, AND YOUR LIFE WITH RESPECT. YOU HAVE BEEN WARNED.

.. note::
    This is initially targeting my Scion FR-S (and by extension the Subaru BRZ
    (probably (but only maybe))). Also, this I'm writing this for myself, and
    I'm only concerned with compatibility with the ``atmega328p`` MCU and the
    ``MCP2515`` CAN module. If this works with something else, then great, but
    frankly I do not care. I'll worry about compatibility with more MCUs, CAN
    modules, and cars in the future, maybe. If you submit a pull request for
    compatibility with your Corolla, great, fantastic; but I'm not going to
    merge it (at least not at this stage). Sorry if this rubs you the wrong
    way, but I have a day job and this is just a passion project.

Copilot is a system for interfacing with a vehicle's CAN bus using an AVR MCU
and an MCP2515 CAN module. Initial target is specifically for the Scion FR-S
(and by extension the Subaru BRZ probably, no guarantees), the ``atmega328p``,
and the ``MCP2515`` CAN module. It provides a method for pull data off the CAN
bus, submitting CAN modules to the bus, pulling data from the ECU, and doing
some minimal abstracted logic.


Backstory
---------

I initially was using an Arduino Uno and the Seeed Studio CAN Shield, but
quickly got tired of the "Arduino" language and the library that is provided
with Seeed. After much Googling, I found very little documentation on how to
use "pure C" to program the Arduino Uno to interface with the MCP2515, so I
decided to just write my own. This is my first "real" project involving
baremetal programming of an Arduino with a shield. I'm sure there are mistakes.
Feel free to critique and I'll fix. Just be warned that I'm not going to worry
about getting this to work with your ``Corolla || Prius || BMW || Civic``.

You will find the following files in the repo:

- ``src/mcp_can.cpp``
- ``src/mcp_can.h``
- ``src/mcp_can_dfs.h``

These are only there while I reverse engineer the Seeed Studio CAN Shield
library. Once I've reimplemented them in "pure C" I'll remove them. Don't
worry, they're licensed under the MIT license so it's all kosher (IANAL).


Goals
-----

The goals of this project are as follows:

- Implement a full library for communicating with the ``MCP2515`` module
- Pull data from the CAN bus
- Push data/messages onto the CAN bus
- Provide abstracted methods for controller vehicle components (i.e., turn
  on headlights, control A/C, etc.)


Requirements
------------

* ``avrdude``
* ``avr-binutils``
* ``avr-gcc``
* ``avr-libc-atmel``


Building/Using
--------------

As said, most of the functionality isn't yet implemented, but you can "use" the
system as follows:

.. code:: bash
    # Clean the repo (just in case)
    $ make clean

    # Build the .hex file
    $ make

    # Program the AVR
    $ make program

You can then connect the system up to your vehicle through the ODBII port. You
can communicate with the AVR over UART/serial (9600 baud).


Limitations
-----------

The current compatibility is as follows:

- Scion FR-S 2013
- ``atmega328p``
- ``mcp2515``

I'm using "pure C" to program an Arduino Uno. As such, it currently requires
the Arduino bootloader. I plan to eventually move to using an ISP to program
the AVR directly, then I may move away from the Arduino platform altogether and
utilize a the hardware without dev boards. Eventually.
