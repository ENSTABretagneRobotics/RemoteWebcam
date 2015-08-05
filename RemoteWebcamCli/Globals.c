// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

SOCKET s1;
CvFont font;
IplImage* image = NULL;
char* databuf = NULL;
BOOL bError = FALSE;

// Parameters.
char cli1address[MAX_BUF_LEN];
char cli1port[MAX_BUF_LEN];
int videoimgwidth = 0; 
int videoimgheight = 0; 
int captureperiod = 0;
int timeout = 0;
BOOL bUDP = FALSE;
BOOL bWindowResizedFromServer = FALSE;
