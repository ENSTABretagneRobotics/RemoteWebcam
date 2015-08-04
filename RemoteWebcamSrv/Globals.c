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
IplImage* frame = NULL;
IplImage* image = NULL;
IplImage* previmage = NULL;
IplImage* detectimage = NULL;
char* databuf = NULL;
int encodeparams[2];
BOOL bStop = FALSE;

char szDevPath[256];
char srvport[256];
int videoimgwidth = 0; 
int videoimgheight = 0; 
int captureperiod = 0;
int timeout = 0;
double angle = 0;
double scale = 1;
int bFlip = FALSE;
BOOL bUDP = FALSE;
int pixcolorchgthreshold = 0; 
int timecompressiondividerthreshold = 0; 
int fullimgperiod = 0; 
int encodequality = 0; 
char encodetype[32];
int method = 0; 
