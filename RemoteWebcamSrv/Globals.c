// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

CvCapture* webcam = NULL;
CHRONO chrono;
CvFont font;
IplImage* image = NULL;
IplImage* previmage = NULL;
IplImage* detectimage = NULL;
char* databuf = NULL;
int jpegparams[2];
BOOL bStop = FALSE;

int camid = 0;
char srvport[MAX_BUF_LEN];
int videoimgwidth = 0; 
int videoimgheight = 0; 
int captureperiod = 0;
int timeout = 0;
BOOL bUDP = FALSE;
int pixcolorchgthreshold = 0; 
int timecompressiondividerthreshold = 0; 
int fullimgperiod = 0; 
int jpegquality = 0; 
int method = 0; 
