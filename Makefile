# Makefile for Linux, designed for Ubuntu 16.04 and Android 4.1.2. 
# You might need to install C/C++ development tools by typing :
#    sudo apt-get install build-essential
# in a terminal.
# You need to install OpenCV 2.4.
# Use dos2unix *.txt to ensure line endings are correct for Linux in the configuration files.

PROGS = RemoteWebcamMultiSrv RemoteWebcamSrv RemoteWebcamCli

CC = gcc
CXX = g++

#CFLAGS += -Wall -Winline -Wextra -g
CFLAGS += -Wall -O3
CFLAGS += -I. -I../OSUtils -I../Extensions/Img
#CFLAGS += -D _DEBUG -D _DEBUG_DISPLAY
#CFLAGS += -D _DEBUG_MESSAGES 
#CFLAGS += -D DISABLE_GUI_REMOTEWEBCAMSRV -D DISABLE_GUI_REMOTEWEBCAMMULTISRV
#CFLAGS += -D USE_OPENCV_HIGHGUI_CPP_API
CFLAGS += -D OPENCV2413
#CFLAGS += -D OPENCV320
#CFLAGS += -D ENABLE_GETTIMEOFDAY_WIN32 -D DISABLE_TIMEZONE_STRUCT_REDEFINITION

CXXFLAGS += $(CFLAGS) -fpermissive

LDFLAGS += -lopencv_core -lopencv_imgproc -lopencv_highgui
#LDFLAGS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio
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

############################# Extensions #############################

CvCore.o: ../Extensions/Img/CvCore.c ../Extensions/Img/CvCore.h OSTime.o
	$(CC) $(CFLAGS) -c $<

CvFiles.o: ../Extensions/Img/CvFiles.c ../Extensions/Img/CvFiles.h CvCore.o
	$(CC) $(CFLAGS) -c $<

CvProc.o: ../Extensions/Img/CvProc.c ../Extensions/Img/CvProc.h CvCore.o
	$(CC) $(CFLAGS) -c $<

CvDraw.o: ../Extensions/Img/CvDraw.c ../Extensions/Img/CvDraw.h CvCore.o
	$(CC) $(CFLAGS) -c $<

CvDisp.o: ../Extensions/Img/CvDisp.c ../Extensions/Img/CvDisp.h CvCore.o
	$(CC) $(CFLAGS) -c $<

############################# PROGS #############################

RemoteWebcamMultiSrv/Globals.o: RemoteWebcamMultiSrv/Globals.cpp CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSTimer.o OSEv.o OSCriticalSection.o OSThread.o OSMisc.o OSTime.o OSCore.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

RemoteWebcamMultiSrv/Main.o: RemoteWebcamMultiSrv/Main.cpp RemoteWebcamMultiSrv/Globals.o
	$(CXX) $(CXXFLAGS) -c $< -o $@

RemoteWebcamMultiSrv: RemoteWebcamMultiSrv/Main.o RemoteWebcamMultiSrv/Globals.o CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSTimer.o OSEv.o OSCriticalSection.o OSThread.o OSMisc.o OSTime.o OSCore.o
	$(CXX) $(CXXFLAGS) -o RemoteWebcamMultiSrv/$@ $^ $(LDFLAGS)

RemoteWebcamSrv/Globals.o: RemoteWebcamSrv/Globals.c CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamSrv/Main.o: RemoteWebcamSrv/Main.c RemoteWebcamSrv/Globals.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamSrv: RemoteWebcamSrv/Main.o RemoteWebcamSrv/Globals.o CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o RemoteWebcamSrv/$@ $^ $(LDFLAGS)

RemoteWebcamCli/Globals.o: RemoteWebcamCli/Globals.c CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli/Main.o: RemoteWebcamCli/Main.c RemoteWebcamCli/Globals.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli: RemoteWebcamCli/Main.o RemoteWebcamCli/Globals.o CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -o RemoteWebcamCli/$@ $^ $(LDFLAGS)

clean:
	rm -f *.o *.obj core *.gch RemoteWebcamMultiSrv/*.o RemoteWebcamSrv/*.o RemoteWebcamCli/*.o RemoteWebcamMultiSrv/RemoteWebcamMultiSrv RemoteWebcamSrv/RemoteWebcamSrv RemoteWebcamCli/RemoteWebcamCli
