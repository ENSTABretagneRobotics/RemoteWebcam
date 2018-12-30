This folder contains programs used to get a realtime remote access to a webcam. It is a TCP server opening a webcam and sending images to a TCP client optionally using :
_ JPEG compression provided by the OpenCV function cvEncodeImage() and with a time compression based on movement detection.
_ No compression.
_ MJPEG (compatible with e.g. VLC, Google Chrome (modify mjpeg.html to test), check also mjpeg.py for a sample client in Python using OpenCV). This is the default.

It is recommended to be connected with an administrator account, deactivate any antivirus, firewall or other settings that would block network ports. On Windows 7/8/10, choose Work/Private for the network type if asked when connecting to the different networks (avoid Public type, to check and change go to Network and Sharing Center and click on Public to be able to change to Work/Private, or go to PC Settings\Network, select the desired network and enable the option Find devices and content/Make this PC discoverable).
Check also proxy settings of Internet Explorer, and any other installed browser.

It should work with the following software :
*** USER and DEVEL ***
_ Windows 10 Professional 64 bit
_ Kinect v2 SDK (not compatible with Visual Studio 2008 and Windows XP)
*** DEVEL ***
_ Visual Studio 2017
_ OpenCV 2.4.13 (see http://www.ensta-bretagne.fr/lebars/Share/setup_vs2017_opencv2413.pdf )

It is also supposed to be compatible with Linux with equivalent prerequisites, although not fully tested, see Makefile.

Restart the computer after installation.

The following are required sources dependencies :  
_ ..\OSUtils : Windows/Linux abstraction layer (threads, network, serial port, time management...) as well as other useful functions.
_ ..\Extensions : some image functions.

Please do not try to build the program in a directory that contains commas! Mind the line endings in the configuration files depending on the OS (use e.g. the command dos2unix *.txt to convert line endings for Linux)!

If recording does not work, try to specify another codec in RemoteWebcamMultiSrv.txt.

See also https://github.com/ENSTABretagneRobotics/Android and http://www.ensta-bretagne.fr/lebars/utilities/.
