RemoteWebcam
============
This folder contains programs used to get a realtime remote access to a webcam. It is a TCP server (RemoteWebcamMultiSrv) opening a webcam and sending images to a TCP client (RemoteWebcamCli, only compatible with OpenCV < 4) optionally using:
- JPEG compression provided by the OpenCV function cvEncodeImage() and with a time compression based on movement detection.
- No compression.
- MJPEG (compatible with e.g. VLC, Google Chrome (modify `mjpeg.html` to test), check also `mjpeg.py` for a sample client in Python using OpenCV, note that RemoteWebcamCli does not support MJPEG). This is the default.

Change the desired parameters in the configuration files (`RemoteWebcamMultiSrv.txt`, `RemoteWebcamCli.txt`, etc.) if necessary. 

It should work with the following software:
*** USER and DEVEL ***
- Windows 10 Professional 64 bit
- Kinect v2 SDK (not compatible with Visual Studio 2008 and Windows XP)
*** DEVEL ***
- Visual Studio 2022
- OpenCV 4.10.0 (see http://www.ensta-bretagne.fr/lebars/Share/setup_vs_opencv.pdf and adapt the instructions to the desired OpenCV version, see https://community.chocolatey.org/packages/libopencv-dev/4.10.0.20240808 )

It is also supposed to be compatible with Linux with equivalent prerequisites (see e.g. https://www.ensta-bretagne.fr/lebars/Share/setup_qt_opencv.pdf ), see `CMakeLists.txt` or `Makefile`.

Restart the computer after installation.

The following are required sources dependencies:  
- `..\OSUtils`: Windows/Linux abstraction layer (threads, network, serial port, time management...) as well as other useful functions (see https://github.com/ENSTABretagneRobotics/OSUtils )
- `..\Extensions`: some image functions (see https://github.com/ENSTABretagneRobotics/Extensions )

Known issues:
- It is recommended to be connected with an administrator account, deactivate any antivirus, firewall or other settings that would block network ports, for Work/Private and Public network types if asked.
- Try to change the value of the port (`4014` by default) in `RemoteWebcamMultiSrv.txt`, `mjpeg.html`, `mjpeg.py`, `RemoteWebcamCli.txt` if the communication does not work (e.g. due to the port already used by another program).
- If recording does not work, try to specify another codec for the parameter `szVideoRecordCodec` in `RemoteWebcamMultiSrv.txt`.
- Mind the line endings in the configuration files depending on the OS (use e.g. the command `dos2unix *.txt` to convert line endings for Linux).
- Please avoid building or run the program in a directory that contains commas or other unusual characters.
- Check the web browser and system proxy settings if your network uses a proxy, if necessary.

See also https://github.com/ENSTABretagneRobotics/Android and http://www.ensta-bretagne.fr/lebars/utilities/ .
