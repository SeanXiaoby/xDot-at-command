The AT Firmware provides an AT interface to the Multitech LoRaWAN stack

Dot libraries
------------------

Include one of the following libraries to complete this project.
  * [libmDot-mbed5](http://github.com/MultiTechSystems/libmDot-mbed5/) -> production-ready build for mDot
  * [libmDot-dev-mbed5](http://github.com/MultiTechSystems/libmDot-dev-mbed5/) -> development build for mDot
  * [libxDot-mbed5](http://github.com/MultiTechSystems/libxDot-mbed5/) -> production-ready build for xDot
  * [libxDot-dev-mbed5](http://github.com/MultiTechSystems/libxDot-dev-mbed5/) -> development build for xDot

A changelog for the Dot library can be found [here](https://developer.mbed.org/teams/MultiTech/wiki/Dot-library-change-log).



Firmware update over the air
------------------------------
FOTA support can be compiled into this application by adding "FOTA=1" to the macros array in the mbed_app.json file.
FOTA is only supported on the mDot platform, xDot does not have an external flash for supporting FOTA.

{
    "macros": [
        "FOTA=1"
    ]
}
        