// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#ifndef GLOBALS_H
#define GLOBALS_H

#include "OSMisc.h"
#include "OSNet.h"
#include "CvInc.h"

#if (CV_MAJOR_VERSION < 4)
EXTERN_C SOCKET s1;
EXTERN_C CvFont font;
EXTERN_C IplImage* image;
EXTERN_C char* databuf;
EXTERN_C BOOL bError;

// Parameters.
EXTERN_C char cli1address[MAX_BUF_LEN];
EXTERN_C char cli1port[MAX_BUF_LEN];
EXTERN_C int videoimgwidth; 
EXTERN_C int videoimgheight; 
EXTERN_C int captureperiod;
EXTERN_C int timeout;
EXTERN_C BOOL bWindowResizedFromServer;
EXTERN_C BOOL bUDP;
#endif // (CV_MAJOR_VERSION < 4)

#endif // !GLOBALS_H
