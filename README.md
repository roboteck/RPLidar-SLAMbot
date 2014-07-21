RPLidar-SLAMbot
===============

Evolving codebase for a SLAM-capable robot using the RoboPeak LIDAR sensor

Contents:
---------

All directories and files used by the SLAMbot

## libraries
The files required by the robot to run its sensors and support its functionality.
### breezyslam
Python and C++ files to enable SLAM, released as open-source BreezySLAM ([link](http://home.wlu.edu/~levys/software/breezyslam/)).
### Encoder
Allows precise, high-frequency, low-overhead encoder monitoring ([link](http://www.pjrc.com/teensy/td_libs_Encoder.html)).
### RPLidarDriver
Provides simple methods for retrieving data from the RPLidar sensor ([link](http://rplidar.robopeak.com/subsites/rplidar/download.html)).
### Standard Python libraries
These are all the standard Python libraries that we use in the Base Station code
#### Version-independent libraries
sys, time, threading, serial, struct, numpy, scipy, matplotlib
#### Python2.x-specific libraries
Tkinter, tkMessageBox, Queue
#### Python3.x-specific libraries
tkinter (contains messagebox), queue

## breezySLAM_XBee.py
Contains all base station code to run the slamBot.  BreezySLAM for Python must already be installed on your machine to use this code.  Written in Python 2.7.6, tested and functioning in Python 3.4.0.  Written and tested in Ubuntu 14.04, however should run on all Linux machines.  Have not tested in Windows or OS X.

## breezySLAM_XBee/breezySLAM_XBee.ino
Arduino code for the Seeeduino Mega on our slamBot.  Encoder and RPLidarDriver should be placed in the sketchbook folder to be properly added by the Arduino compiler at compile-time.  Cannot be run on an Arduino with fewer than 4 serial ports if full functionality is to be maintained.

## Linux Custom Baud Hack
Allows you to use non-standard baud rates on a Linux machine (more info: [link](https://groups.google.com/forum/#!msg/ultimaker/BNjPpoJpfrE/Xmbp0XxTWXEJ)).
If you receive the error "Inappropriate ioctl for device", traced back to within pySerial, apply this patch.
### Python2
Contains all the patch information for Python 2.x.
#### pyserial.patch
Created with `diff -u list_ports_posix_old.py list_ports_posix_new.py > listports.patch`.
#### serialposix_old.py
This is our backup of the `serialposix.py` file.
#### serialposix_new.py
This is what your `serialposix.py` file should look like after applying the patch.
### Python3
If you receive the error `TypeError: 'str' does not support the buffer interface`, traced back to within pySerial, apply this patch (more info: [link](http://stackoverflow.com/questions/5471158/typeerror-str-does-not-support-the-buffer-interface)).  The second patch in the patch file addresses this issue (the first is the baud rate hack).
#### pyserial.patch
Created with `diff -u list_ports_posix_old.py list_ports_posix_new.py > listports.patch`.
#### serialposix_old.py
This is our backup of the `serialposix.py` file.
#### serialposix_new.py
This is what your `serialposix.py` file should look like after applying the patch.


## Linux list_ports Hack
Fixes list_ports in Python 3.  Do not apply this patch to pySerial if you are running Python 2.x.
### Python3
If you receive the error `TypeError: can't use a string pattern on a bytes-like object`, traced back to within pySerial, apply this patch (more info: [link](http://stackoverflow.com/questions/5184483/python-typeerror-on-regex)).
#### listports.patch
Created with `diff -u list_ports_posix_old.py list_ports_posix_new.py > listports.patch`.
#### list_ports_posix_old.py
This is our backup of the `list_ports_posix.py` file.
#### list_ports_posix_new.py
This is what your `list_ports_posix.py` file should look like after applying the patch.


## Seeeduino Mega Extra Pins Hack
Enables pin control for digital 70-85 on Seeed Studio's Mega.
Ubuntu has `patch` and `diff` built-in, but if you're using a different OS, we recommend manually replacing the files.
Note that the first patch is optional, but we recommend it as it allows easily switching back to the unmodified Arduino IDE code.
### paths.txt
Contains the location of the files we're modifying, on both Windows and Ubuntu.  Note that your installation may be in a different location.  You will need to find it before applying the patches.
### seeedmega_menuctl.patch
Add "Seeeduino Mega" board type to Arduino IDE menu.
Created with `diff -u boards_old.txt boards_new.txt > seeedmega_menuctl.patch`.

To use:

`cd /usr/share/arduino/hardware/arduino/`

`sudo patch boards.txt < /[path of patch file on your machine]/seeedmega_menuctl.patch`.
#### boards_old.txt
This is our backup of the `boards.txt` file.
#### boards_new.txt
This is what your `boards.txt` file should look like after applying the patch.
### seeedmega_pindef.patch
Add new board type defining the Seeeduino Mega's pin definitions.
Created with `diff -u mega/pins_arduino.h seeed/pins_arduino.h > seeedmega_pindef.patch`.

To use:

`cd /usr/share/arduino/hardware/arduino/variants/`

`sudo cp -R mega seeed`

`sudo patch seeed/pins_arduino.h < /[path of patch file on your machine]/seeedmega_pindef.patch`.
#### mega/pins_arduino.h
This is our backup of the `mega/pins_arduino.h` file.
#### seeed/pins_arduino.h
This is what your new `seeed/pins_arduino.h` file should look like after applying the patch.
### extraPinsControl/extraPinsControl.ino
Example Seeeduino Mega code demonstrating use of extra pins, with some low-level stuff.
