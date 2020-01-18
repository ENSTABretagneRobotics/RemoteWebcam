# Designed for Ubuntu 18.04 (and Android 4.1.2). 
# You might need to install C/C++ development tools and OpenCV 3.2.0 by typing :
#    sudo apt-get install build-essential libopencv-dev
# in a terminal.
# Use dos2unix *.txt to ensure line endings are correct for Linux in the configuration files.

PROGS = RemoteWebcamMultiSrv RemoteWebcamCli

CC = gcc
CXX = g++

#CFLAGS += -g
CFLAGS += -O3
CFLAGS += -Wall -Wextra
#CFLAGS += -Winline
CFLAGS += -I. -I../OSUtils -I../Extensions/Img
#CFLAGS += -D _DEBUG -D _DEBUG_DISPLAY
#CFLAGS += -D _DEBUG_MESSAGES 
#CFLAGS += -D DISABLE_GUI_REMOTEWEBCAMMULTISRV
CFLAGS += -D USE_OPENCV_HIGHGUI_CPP_API
#CFLAGS += -D OPENCV2413
CFLAGS += -D OPENCV320
#CFLAGS += -D OPENCV412

# For MinGW
#CFLAGS += -D ENABLE_GETTIMEOFDAY_WIN32 -D DISABLE_TIMEZONE_STRUCT_REDEFINITION

CXXFLAGS += $(CFLAGS) -fpermissive

# Might be necessary for recent GCC versions...
#CXXFLAGS += -Wno-psabi

# Might be necessary for recent OpenCV versions...
#CXXFLAGS += -std=c++11

# Might be necessary to tweak depending on OpenCV version...
#LDFLAGS += -lopencv_core -lopencv_imgproc -lopencv_highgui
LDFLAGS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio

# For MinGW
#LDFLAGS += -lWinMM -lws2_32 -lpthread -lm

# For Mac OS
#LDFLAGS += -lpthread -lm

# For Linux
LDFLAGS += -lpthread -lrt -lm

default: $(PROGS)

############################# OSUtils #############################

OSCore.o: ../OSUtils/OSCore.c ../OSUtils/OSCore.h
	$(CC) $(CFLAGS) -c $<

OSCriticalSection.o: ../OSUtils/OSCriticalSection.c ../OSUtils/OSCriticalSection.h OSThread.o
	$(CC) $(CFLAGS) -c $<

OSEv.o: ../OSUtils/OSEv.c ../OSUtils/OSEv.h OSThread.o
	$(CC) $(CFLAGS) -c $<

OSMisc.o: ../OSUtils/OSMisc.c ../OSUtils/OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

OSNet.o: ../OSUtils/OSNet.c ../OSUtils/OSNet.h OSThread.o
	$(CC) $(CFLAGS) -c $<

OSThread.o: ../OSUtils/OSThread.c ../OSUtils/OSThread.h OSTime.o
	$(CC) $(CFLAGS) -c $<

OSTime.o: ../OSUtils/OSTime.c ../OSUtils/OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

OSTimer.o: ../OSUtils/OSTimer.c ../OSUtils/OSTimer.h OSEv.o
	$(CC) $(CFLAGS) -c $<

############################# PROGS #############################

RemoteWebcamMultiSrv/Globals.o: RemoteWebcamMultiSrv/Globals.cpp OSNet.o OSTimer.o OSEv.o OSCriticalSection.o OSThread.o OSMisc.o OSTime.o OSCore.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

RemoteWebcamMultiSrv/Main.o: RemoteWebcamMultiSrv/Main.cpp RemoteWebcamMultiSrv/Globals.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

RemoteWebcamMultiSrv: RemoteWebcamMultiSrv/Main.o RemoteWebcamMultiSrv/Globals.o OSNet.o OSTimer.o OSEv.o OSCriticalSection.o OSThread.o OSMisc.o OSTime.o OSCore.o
	$(CXX) $(CXXFLAGS) -o RemoteWebcamMultiSrv/$@ $^ $(LDFLAGS)

RemoteWebcamCli/Globals.o: RemoteWebcamCli/Globals.c OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli/Main.o: RemoteWebcamCli/Main.c RemoteWebcamCli/Globals.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli: RemoteWebcamCli/Main.o RemoteWebcamCli/Globals.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o RemoteWebcamCli/$@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.obj core *.gch RemoteWebcamMultiSrv/*.o RemoteWebcamCli/*.o RemoteWebcamMultiSrv/RemoteWebcamMultiSrv RemoteWebcamCli/RemoteWebcamCli
