#include "Globals.h"

int LoadConfig()
{
	FILE* file = NULL;
	char line[256];
	double d0 = 0;

	// Default values.
	memset(szDevPath, 0, sizeof(szDevPath));
	sprintf(szDevPath, "0");
	memset(srvport, 0, sizeof(srvport));
	sprintf(srvport, "4014");
	videoimgwidth = 640; 
	videoimgheight = 480; 
	captureperiod = 40;
	timeout = 5;
	bForceSoftwareResize = 1;
	hscale = 1;
	vscale = 1;
	angle = 0*M_PI/180.0;
	bFlip = 0;
	bUDP = 0;
	pixcolorchgthreshold = 12; 
	timecompressiondividerthreshold = 50;
	fullimgperiod = 1000; 
	encodequality = 50;
	memset(encodetype, 0, sizeof(encodetype));
	sprintf(encodetype, ".JPEG");
	method = 0;

#ifdef __ANDROID__
	file = fopen("/storage/sdcard0/download/RemoteWebcamSrv.txt", "r");
#else
	file = fopen("RemoteWebcamSrv.txt", "r");
#endif // __ANDROID__
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "[^\n]%255s", szDevPath) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%255s", srvport) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgwidth) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgheight) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &captureperiod) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &timeout) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bForceSoftwareResize) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &hscale) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &vscale) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%lf", &d0) != 1) printf("Invalid configuration file.\n");
		angle = d0*M_PI/180.0;
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bFlip) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bUDP) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &pixcolorchgthreshold) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &timecompressiondividerthreshold) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &fullimgperiod) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &encodequality) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%31s", encodetype) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &method) != 1) printf("Invalid configuration file.\n");
		if (fclose(file) != EXIT_SUCCESS) printf("fclose() failed.\n");
	}
	else
	{
		printf("Configuration file not found.\n");
	}

	if (videoimgwidth <= 0)
	{
		printf("Invalid parameter : videoimgwidth.\n");
		videoimgwidth = 640;
	}
	if (videoimgheight <= 0)
	{
		printf("Invalid parameter : videoimgheight.\n");
		videoimgheight = 480;
	}
	if (captureperiod < 0)
	{
		printf("Invalid parameter : captureperiod.\n");
		captureperiod = 40;
	}
	if (hscale == 0)
	{
		printf("Invalid parameter : hscale.\n");
		hscale = 1;
	}
	if (vscale == 0)
	{
		printf("Invalid parameter : vscale.\n");
		vscale = 1;
	}
	if (timecompressiondividerthreshold < 1)
	{
		printf("Invalid parameter : timecompressiondividerthreshold.\n");
		timecompressiondividerthreshold = 50;
	}

	return EXIT_SUCCESS;
}

int MovementDetection(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes)
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = (unsigned char*)detectimg->imageData;
	CvMat* mat = NULL;

	// First compute the static compression to be able to compare with the time compression.
	mat = cvEncodeImage(encodetype, img, encodeparams);
	if (mat == NULL)
	{
		printf("cvMat() failed.\n");
		return EXIT_FAILURE;
	}

	// Reserved for number of bytes sent and image dimensions.
	count = 3*sizeof(unsigned int); 

	i = img->imageSize;
	while (i -= 3)
	{
		indexb = i;
		indexg = i+1;
		indexr = i+2;

		b = data[indexb]-prevdata[indexb];
		g = data[indexg]-prevdata[indexg];
		r = data[indexr]-prevdata[indexr];
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthreshold)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			// Show in green pixels that significantly changed.
			detectdata[indexb] = 0;
			detectdata[indexg] = 255;
			detectdata[indexr] = 0;
		}

		// A full image will be sent if the time compression is worse than the static one.
		if (count > (unsigned int)(mat->rows*mat->cols)) break;
	}	

	if (count > (unsigned int)(mat->rows*mat->cols)) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Static compressed image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
		// Full image data (with static compression).
		memcpy(buf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
		count = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
	}
	else
	{
		val = count; // Number of bytes sent.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&img->width, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&img->height, sizeof(unsigned int));
	}

	*pnbBytes = (int)count;

	cvReleaseMat(&mat);

	return EXIT_SUCCESS;
}

int MovementDetection2(IplImage* previmg, IplImage* img, IplImage* detectimg, char* buf, int* pnbBytes)
{
	unsigned int i = 0, val = 0, count = 0, indexb = 0, indexg = 0, indexr = 0;
	int r = 0, g = 0, b = 0;
	unsigned char* prevdata = (unsigned char*)previmg->imageData;
	unsigned char* data = (unsigned char*)img->imageData;
	unsigned char* detectdata = (unsigned char*)detectimg->imageData;
	CvMat* mat = NULL;

	// Reserved for number of bytes sent and image dimensions.
	count = 3*sizeof(unsigned int); 

	i = img->imageSize;
	while (i -= 3)
	{
		indexb = i;
		indexg = i+1;
		indexr = i+2;

		b = data[indexb]-prevdata[indexb];
		g = data[indexg]-prevdata[indexg];
		r = data[indexr]-prevdata[indexr];
		if ((abs(b)+abs(g)+abs(r)) > 3*pixcolorchgthreshold)
		{
			// Blue index value of the pixel.
			memcpy(buf+count, (char*)&i, sizeof(unsigned int));
			count += sizeof(unsigned int);
			// BGR values.
			memcpy(buf+count, img->imageData+i, 3*sizeof(char));
			count += 3*sizeof(char);
			// Show in green pixels that significantly changed.
			detectdata[indexb] = 0;
			detectdata[indexg] = 255;
			detectdata[indexr] = 0;
		}

		// A full image will be sent if the time compression is bad.
		if (count > (unsigned int)(img->imageSize/timecompressiondividerthreshold)) break;
	}	

	if (count > (unsigned int)(img->imageSize/timecompressiondividerthreshold)) 
	{
		val = UINT_MAX; // Special number to indicate a full image.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		mat = cvEncodeImage(encodetype, img, encodeparams);
		if (mat == NULL)
		{
			printf("cvMat() failed.\n");
			return EXIT_FAILURE;
		}
		// Static compressed image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
		// Full image data (with static compression).
		memcpy(buf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
		count = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
		cvReleaseMat(&mat);
	}
	else
	{
		val = count; // Number of bytes sent.
		memcpy(buf, (char*)&val, sizeof(unsigned int));
		// Image dimensions.
		memcpy(buf+sizeof(unsigned int), (char*)&img->width, sizeof(unsigned int));
		memcpy(buf+2*sizeof(unsigned int), (char*)&img->height, sizeof(unsigned int));
	}

	*pnbBytes = (int)count;

	return EXIT_SUCCESS;
}

void CleanUp(void)
{
	StopChronoQuick(&chrono);
	cvReleaseImage(&detectimage);
	cvReleaseImage(&previmage);
}

int handlecli(SOCKET sockcli, void* pParam)
{
	double m[6]; // For rotation...
	CvMat M = cvMat(2, 3, CV_64F, m); // For rotation...
	int nbBytes = 0;
	char szText[256];
	unsigned int val = 0;
	CvMat* mat = NULL;
	BOOL bForceSendFullImg = TRUE;
	BOOL bInitDone = FALSE;
	char httpbuf[2048];
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
	int c = 0;
#endif // DISABLE_GUI_REMOTEWEBCAMSRV

	UNREFERENCED_PARAMETER(pParam);

	previmage = cvCloneImage(image);
	if (!previmage)	
	{
		printf("Error copying an image.\n");
		return EXIT_FAILURE;
	}
	detectimage = cvCloneImage(image);
	if (!detectimage)	
	{
		printf("Error copying an image.\n");
		cvReleaseImage(&previmage);
		return EXIT_FAILURE;
	}

	StartChrono(&chrono);

	if (bUDP)
	{
		int broadcastenabled = 1;
		struct sockaddr_in sa;
		socklen_t salen = sizeof(struct sockaddr_in);

		setsockopt(sockcli, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastenabled, sizeof(broadcastenabled));

		memset(&sa, 0, sizeof(sa));

		sa.sin_family = AF_INET;
		//sa.sin_addr.s_addr = INADDR_BROADCAST;
		sa.sin_addr.s_addr = inet_addr("192.168.1.255");
		sa.sin_port = htons((unsigned short)atoi(srvport));

		recvfromall(sockcli, databuf, strlen("HELLO")+1, (struct sockaddr*)&sa, &salen);
		connect(sockcli, (struct sockaddr*)&sa, salen);
		sprintf(databuf, "OK");
		sendtoall(sockcli, databuf, strlen("OK")+1, (struct sockaddr*)&sa, salen);
		recvall(sockcli, databuf, strlen("HELLO")+1);
		sprintf(databuf, "OK");
		sendall(sockcli, databuf, strlen("OK")+1);
	}

	for (;;)
	{
		fd_set sock_set;
		int iResult = SOCKET_ERROR;
		struct timeval tv;

		tv.tv_sec = (long)(timeout/1000);
		tv.tv_usec = (long)((timeout%1000)*1000);

		// Initialize a fd_set and add the socket to it.
		FD_ZERO(&sock_set); 
		FD_SET(sockcli, &sock_set);

		iResult = select((int)sockcli+1, NULL, &sock_set, NULL, &tv);

		// Remove the socket from the set.
		// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
		// sufficient to know what happened.
		FD_CLR(sockcli, &sock_set); 

		switch (iResult)
		{
		case SOCKET_ERROR:
			CleanUp();
			return EXIT_FAILURE;
		case 0:
			// The timeout on select() occured.
			break;
		default:
			frame = cvQueryFrame(webcam);
			if (!frame)
			{
				printf("Error getting an image from the webcam.\n");
				CleanUp();
				return EXIT_FAILURE;
			}
			if (bForceSoftwareResize) cvResize(frame, resizedframe, CV_INTER_LINEAR);
			else resizedframe = frame;

			if ((hscale == 1)&&(vscale == 1)&&(angle == 0))
			{
				if (bFlip) cvFlip(resizedframe, image, 1); else cvCopy(resizedframe, image, 0);
			}
			else
			{
				// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.
				//double hscale = 0.5, vscale = 1.33*0.5;
				m[0] = cos(angle)/hscale;
				m[1] = sin(angle)/hscale;
				m[3] = -sin(angle)/vscale;
				m[4] = cos(angle)/vscale;
				m[2] = (1-cos(angle)/hscale)*resizedframe->width*0.5-(sin(angle)/hscale)*resizedframe->height*0.5;  
				m[5] = (sin(angle)/vscale)*resizedframe->width*0.5+(1-cos(angle)/vscale)*resizedframe->height*0.5;
				cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP, cvScalarAll(0));

				//double hscale = 1.33*0.5, vscale = 0.5;
				//m[0] = hscale*cos(-angle);
				//m[1] = hscale*sin(-angle);
				//m[3] = vscale*-sin(-angle);
				//m[4] = vscale*cos(-angle);
				//m[2] = (1-hscale*cos(-angle))*resizedframe->width*0.5-hscale*sin(-angle)*resizedframe->height*0.5;  
				//m[5] = vscale*sin(-angle)*resizedframe->width*0.5+(1-vscale*cos(-angle))*resizedframe->height*0.5;
				////cvGetQuadrangleSubPix(resizedframe, image, &M);
				////cv2DRotationMatrix(cvPoint2D32f(resizedframe->width*0.5,resizedframe->height*0.5), -angle*180.0/M_PI, scale, &M);
				//cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

				if (bFlip) cvFlip(image, NULL, 1);
			}

			cvCopy(image, detectimage, NULL);

			if (
				((method == 0)||(method == 1))&&
				((GetTimeElapsedChronoQuick(&chrono) > 0.001*fullimgperiod)||bForceSendFullImg)
				) 
			{
				StopChronoQuick(&chrono);
				bForceSendFullImg = FALSE;
				val = UINT_MAX; // Special number to indicate a full image with static compression.
				memcpy(databuf, (char*)&val, sizeof(unsigned int));
				mat = cvEncodeImage(encodetype, image, encodeparams);
				if (mat == NULL)
				{
					printf("cvMat() failed.\n");
					CleanUp();
					return EXIT_FAILURE;
				}
				// Static compressed image dimensions.
				memcpy(databuf+sizeof(unsigned int), (char*)&mat->rows, sizeof(unsigned int));
				memcpy(databuf+2*sizeof(unsigned int), (char*)&mat->cols, sizeof(unsigned int));
				// Full image data (with static compression).
				memcpy(databuf+3*sizeof(unsigned int), mat->data.ptr, (size_t)(mat->rows*mat->cols));
				nbBytes = 3*sizeof(unsigned int)+(unsigned int)(mat->rows*mat->cols);
				cvReleaseMat(&mat);
				sprintf(szText, "Periodic complete image : %d bytes", nbBytes);
				cvPutText(detectimage, szText, cvPoint(0,48), &font, CV_RGB(255,0,0));
				StartChrono(&chrono);
			}
			else
			{
				switch (method)
				{
				case 0:
					if (MovementDetection(previmage, image, detectimage, databuf, &nbBytes) != EXIT_SUCCESS)
					{
						CleanUp();
						return EXIT_FAILURE;
					}
					break;
				case 1:
					if (MovementDetection2(previmage, image, detectimage, databuf, &nbBytes) != EXIT_SUCCESS)
					{
						CleanUp();
						return EXIT_FAILURE;
					}
					break;
				case 2:	
					val = (unsigned int)image->imageSize+3*sizeof(unsigned int); // Special number to indicate a full image without compression.
					memcpy(databuf, (char*)&val, sizeof(unsigned int));
					// Image dimensions.
					memcpy(databuf+sizeof(unsigned int), (char*)&image->width, sizeof(unsigned int));
					memcpy(databuf+2*sizeof(unsigned int), (char*)&image->height, sizeof(unsigned int));
					// Full image data without compression.
					memcpy(databuf+3*sizeof(unsigned int), image->imageData, (size_t)image->imageSize);
					nbBytes = val;
					break;
				case 3:	
					nbBytes = 0;
					if (!bInitDone)
					{
						// Receive the GET request, but do not analyze it...
						memset(httpbuf, 0, sizeof(httpbuf));
						if (recv(sockcli, httpbuf, sizeof(httpbuf), 0) <= 0)
						{
							printf("recv() failed.\n");
							CleanUp();
							return EXIT_FAILURE;
						}
						memset(httpbuf, 0, sizeof(httpbuf));
						sprintf(httpbuf, 
							"HTTP/1.1 200 OK\r\n"
							"Server: RemoteWebcamSrv\r\n"
							//"Connection: close\r\n"
							//"Max-Age: 0\r\n"
							//"Expires: 0\r\n"
							//"Cache-Control: no-cache, private, no-store, must-revalidate, pre-check = 0, post-check = 0, max-age = 0\r\n"
							//"Pragma: no-cache\r\n"
							"Content-Type: multipart/x-mixed-replace; boundary=--boundary\r\n"
							//"Media-type: image/jpeg\r\n"
							"\r\n");
						memcpy(databuf, httpbuf, strlen(httpbuf));
						nbBytes += strlen(httpbuf);
						bInitDone = TRUE;
					}
					mat = cvEncodeImage(encodetype, image, encodeparams);
					if (mat == NULL)
					{
						printf("cvMat() failed.\n");
						CleanUp();
						return EXIT_FAILURE;
					}
					memset(httpbuf, 0, sizeof(httpbuf));
					sprintf(httpbuf, 
						"--boundary\r\n"
						"Content-Type: image/jpeg\r\n"
						"Content-Length: %d\r\n"
						"\r\n", mat->rows*mat->cols);
					memcpy(databuf+nbBytes, httpbuf, strlen(httpbuf));
					nbBytes += strlen(httpbuf);
					// Full image data (with static compression).
					memcpy(databuf+nbBytes, mat->data.ptr, (size_t)(mat->rows*mat->cols));
					nbBytes += (mat->rows*mat->cols);
					cvReleaseMat(&mat);
					break;
				default:
					printf("Invalid parameter.\n");
					CleanUp();
					return EXIT_FAILURE;
					break;
				}
				sprintf(szText, "Data transmitted : %d bytes", nbBytes);
				cvPutText(detectimage, szText, cvPoint(0,32), &font, CV_RGB(255,0,0));
			}
			if (sendall(sockcli, databuf, nbBytes) != EXIT_SUCCESS)
			{
				CleanUp();
				return EXIT_FAILURE;
			}
			sprintf(szText, "Uncompressed size : %d bytes", image->imageSize);
			cvPutText(detectimage, szText, cvPoint(0,16), &font, CV_RGB(255,0,0));
			cvCopy(image, previmage, NULL);

#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
			cvShowImage("Detection", detectimage);

			c = cvWaitKey(captureperiod);
			if ((char)c == 27)
			{
				CleanUp();
				return EXIT_SUCCESS;
			}
#else
			mSleep(captureperiod);
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
			break;
		}

		if (bStop) break;
	}

	StopChronoQuick(&chrono);
	cvReleaseImage(&detectimage);
	cvReleaseImage(&previmage);

	return EXIT_SUCCESS;
}

int quitall()
{
	bStop = TRUE;
	mSleep(15000);
	exit(EXIT_SUCCESS);
}

#ifdef __ANDROID__
int AppNativeQuit()
{
	return quitall();
}

int AppNativeMain()
{
#else
#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	int argc = __argc;
	char** argv = __argv;
#else
int main(int argc, char* argv[])
{
#endif // defined(_WIN32) && !defined(_DEBUG)
#endif // __ANDROID__
	double m[6]; // For rotation...
	CvMat M = cvMat(2, 3, CV_64F, m); // For rotation...
	int i = 0;

#ifndef __ANDROID__
	INIT_DEBUG;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif // __ANDROID__

	LoadConfig();

	if (strncmp(encodetype, ".JPEG", min(strlen(encodetype), strlen(".JPEG"))+1) == 0)
	{
		encodeparams[0] = CV_IMWRITE_JPEG_QUALITY;
		encodeparams[1] = encodequality;
	}
	else if (strncmp(encodetype, ".PNG", min(strlen(encodetype), strlen(".PNG"))+1) == 0)
	{
		encodeparams[0] = CV_IMWRITE_PNG_COMPRESSION;
		encodeparams[1] = encodequality;
	}
	else
	{
		encodeparams[0] = CV_IMWRITE_PXM_BINARY;
		encodeparams[1] = encodequality;
	}

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0, 1, 8);

	if ((strlen(szDevPath) == 1)&&(isdigit((unsigned char)szDevPath[0])))
	{
		webcam = cvCreateCameraCapture(atoi(szDevPath));
	}
	else
	{
		webcam = cvCreateFileCapture(szDevPath);
	}
	if (!webcam) 
	{
		printf("Error opening the webcam.\n");
		return EXIT_FAILURE;
	}

	cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_WIDTH, videoimgwidth);
	cvSetCaptureProperty(webcam, CV_CAP_PROP_FRAME_HEIGHT, videoimgheight);

	// Sometimes the first images are bad, so wait a little bit and take
	// several images in the beginning.
	i = 0;
	while (i < 2)
	{
		mSleep(500);
		frame = cvQueryFrame(webcam);
		i++;
	}

	frame = cvQueryFrame(webcam);
	if (!frame)
	{
		printf("Error getting an image from the webcam.\n");
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}
	
	if (bForceSoftwareResize) 
	{
		resizedframe = cvCreateImage(cvSize(videoimgwidth,videoimgheight), frame->depth, frame->nChannels);
		if (!resizedframe)	
		{
			printf("Error creating an image.\n");
			cvReleaseCapture(&webcam);
			return EXIT_FAILURE;
		}
		cvResize(frame, resizedframe, CV_INTER_LINEAR);
	}
	else resizedframe = frame;

	image = cvCloneImage(resizedframe);
	if (!image)	
	{
		printf("Error copying an image.\n");
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}

	if ((angle == 0)&&(hscale == 1)&&(vscale == 1))
	{
		if (bFlip) cvFlip(resizedframe, image, 1); else cvCopy(resizedframe, image, 0);
	}
	else
	{
		// Create a map_matrix, where the left 2x2 matrix is the transform and the right 2x1 is the dimensions.
		//double hscale = 0.5, vscale = 1.33*0.5;
		m[0] = cos(angle)/hscale;
		m[1] = sin(angle)/hscale;
		m[3] = -sin(angle)/vscale;
		m[4] = cos(angle)/vscale;
		m[2] = (1-cos(angle)/hscale)*resizedframe->width*0.5-(sin(angle)/hscale)*resizedframe->height*0.5;  
		m[5] = (sin(angle)/vscale)*resizedframe->width*0.5+(1-cos(angle)/vscale)*resizedframe->height*0.5;
		cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS+CV_WARP_INVERSE_MAP, cvScalarAll(0));

		//double hscale = 1.33*0.5, vscale = 0.5;
		//m[0] = hscale*cos(-angle);
		//m[1] = hscale*sin(-angle);
		//m[3] = vscale*-sin(-angle);
		//m[4] = vscale*cos(-angle);
		//m[2] = (1-hscale*cos(-angle))*resizedframe->width*0.5-hscale*sin(-angle)*resizedframe->height*0.5;  
		//m[5] = vscale*sin(-angle)*resizedframe->width*0.5+(1-vscale*cos(-angle))*resizedframe->height*0.5;
		////cvGetQuadrangleSubPix(resizedframe, image, &M);
		////cv2DRotationMatrix(cvPoint2D32f(resizedframe->width*0.5,resizedframe->height*0.5), -angle*180.0/M_PI, scale, &M);
		//cvWarpAffine(resizedframe, image, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0));

		if (bFlip) cvFlip(image, NULL, 1);
	}

#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
	cvNamedWindow("Detection", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Detection", 0, 0);
	cvShowImage("Detection", image);
	cvWaitKey(10);
#else
	mSleep(10);
#endif // DISABLE_GUI_REMOTEWEBCAMSRV

	databuf = (char*)calloc(image->imageSize+3*sizeof(unsigned int), sizeof(char));
	if (!databuf)	
	{
		printf("calloc() failed.\n");
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
		cvDestroyWindow("Detection");
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
		cvReleaseImage(&image);
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}

	if (
		(bUDP&&(LaunchUDPSrv(srvport, handlecli, NULL) != EXIT_SUCCESS))||
		((!bUDP)&&(LaunchSingleCliTCPSrv(srvport, handlecli, NULL) != EXIT_SUCCESS))
		)
	{
		printf("Error launching the server.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		free(databuf);
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
		cvDestroyWindow("Detection");
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
		cvReleaseImage(&image);
		if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
		cvReleaseCapture(&webcam);
		return EXIT_FAILURE;
	}

	free(databuf);
#ifndef DISABLE_GUI_REMOTEWEBCAMSRV
	cvDestroyWindow("Detection");
#endif // DISABLE_GUI_REMOTEWEBCAMSRV
	cvReleaseImage(&image);
	if (bForceSoftwareResize) cvReleaseImage(&resizedframe);
	cvReleaseCapture(&webcam);

	return EXIT_SUCCESS;
}
