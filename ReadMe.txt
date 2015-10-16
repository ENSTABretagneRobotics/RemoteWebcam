This folder contains programs used to get a realtime remote access to a webcam. It is a TCP server opening a webcam and sending images to a TCP client optionally using :
_ JPEG compression provided by the OpenCV function cvEncodeImage() and with a time compression based on movement detection 
_ No compression
_ MJPEG (compatible with e.g. VLC, Firefox, Chrome (modify mjpeg.html to test))

It is recommended to be connected with an administrator account, deactivate any antivirus, firewall or other settings that would block network ports. On Windows 7-8-10, choose Work for the network type if asked when connecting to the different networks (avoid Public type, to check and change go to Network and Sharing Center and click on Public to be able to change to Work).
Check also proxy settings of Internet Explorer, and any other installed browser.

It should work with the following software :
*** USER and DEVEL ***
_ Windows XP Professional SP3 32 bit/Windows 8 Professional 64 bit
_ Kinect v2 SDK (not available for Visual Studio 2008 and Windows XP)
*** DEVEL ***
_ Visual Studio 2008 SP1/Visual Studio 2012
_ OpenCV 2.4.9 (download http://www.ensta-bretagne.fr/lebars/Share/OpenCV2.4.9.zip and extract in C:)

It is also supposed to be compatible with Linux 2.6 with equivalent prerequisites, although not fully tested.

Restart the computer after installation.

The following are required sources dependencies :  
_ ..\OSUtils : Windows/Linux abstraction layer (threads, network, serial port, time management...) as well as other useful functions.
_ ..\Extensions : some image functions.

See also https://github.com/ENSTABretagneRobotics/Android and http://www.ensta-bretagne.fr/lebars/utilities/
