# Makefile for Linux [to test with Ubuntu 10.10, tested with Android 4.1.2]. 
# You might need to install C/C++ development tools by typing :
#    sudo apt-get install build-essential
# in a terminal.
# You need to install OpenCV 2.4.9.

PROGS = RemoteWebcamSrv RemoteWebcamCli

CC = gcc
CFLAGS += -Wall -Winline -Wextra -g
#CFLAGS += -Wall -O3
CFLAGS += -I. -I../OSUtils -I../Extensions/Img
CFLAGS += -D _DEBUG -D _DEBUG_DISPLAY -D _DEBUG_MESSAGES 
CFLAGS += -D ENABLE_KILL_THREAD -D ENABLE_CANCEL_THREAD 
CFLAGS += -D ENABLE_SET_THREAD_DEFAULT_PROCESSOR -D ENABLE_PRIORITY_HANDLING
CFLAGS += -D ENABLE_WAIT_FOR_THREAD_TIMEOUT
CFLAGS += -D OPENCV242
LDFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LDFLAGS += -lpthread -lrt -lm 

default: $(PROGS)

############################# OSUtils #############################

OSCore.o: ../OSUtils/OSCore.c ../OSUtils/OSCore.h
	$(CC) $(CFLAGS) -c $<

OSTime.o: ../OSUtils/OSTime.c ../OSUtils/OSTime.h OSCore.o
	$(CC) $(CFLAGS) -c $<

OSMisc.o: ../OSUtils/OSMisc.c ../OSUtils/OSMisc.h OSTime.o
	$(CC) $(CFLAGS) -c $<

OSNet.o: ../OSUtils/OSNet.c ../OSUtils/OSNet.h OSTime.o
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

RemoteWebcamSrv/Globals.o: RemoteWebcamSrv/Globals.c CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamSrv/Main.o: RemoteWebcamSrv/Main.c RemoteWebcamSrv/Globals.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamSrv: RemoteWebcamSrv/Main.o RemoteWebcamSrv/Globals.o CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o RemoteWebcamSrv/$@ $^

RemoteWebcamCli/Globals.o: RemoteWebcamCli/Globals.c CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli/Main.o: RemoteWebcamCli/Main.c RemoteWebcamCli/Globals.o
	$(CC) $(CFLAGS) -c $< -o $@

RemoteWebcamCli: RemoteWebcamCli/Main.o RemoteWebcamCli/Globals.o CvDisp.o CvDraw.o CvProc.o CvFiles.o CvCore.o OSNet.o OSMisc.o OSTime.o OSCore.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o RemoteWebcamCli/$@ $^

clean:
	rm -f *.o *.obj core *.gch RemoteWebcamSrv/RemoteWebcamSrv RemoteWebcamCli/RemoteWebcamCli
