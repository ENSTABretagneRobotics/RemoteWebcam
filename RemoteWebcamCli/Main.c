#include "Globals.h"

int LoadConfig()
{
	FILE* file = NULL;
	char line[MAX_BUF_LEN];

	// Default values.
	memset(cli1address, 0, sizeof(cli1address));
	sprintf(cli1address, "127.0.0.1");
	memset(cli1port, 0, sizeof(cli1port));
	sprintf(cli1port, "27254");
	videoimgwidth = 640; 
	videoimgheight = 480; 
	captureperiod = 5;
	timeout = 0;
	bUDP = FALSE;
	bDynamicWindowResizing = FALSE;

	file = fopen("RemoteWebcamCli.txt", "r");
	if (file != NULL)
	{
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%255s", cli1address) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%255s", cli1port) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgwidth) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &videoimgheight) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &captureperiod) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &timeout) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bUDP) != 1) printf("Invalid configuration file.\n");
		if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		if (sscanf(line, "%d", &bDynamicWindowResizing) != 1) printf("Invalid configuration file.\n");

		//if (fgets3(file, line, sizeof(line)) == NULL) printf("Invalid configuration file.\n");
		//if (sscanf(line, "%lf", &val) != 1) printf("Invalid configuration file.\n");

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
		captureperiod = 5;
	}

	return EXIT_SUCCESS;
}

int recvdecode(void)
{
	unsigned int header[3];
	unsigned int val = 0;
	int nbBytes = 0;
	int i = 0;
	CvMat* mat = NULL;
	IplImage* imagenew = image;
	char* databufnew = databuf;

	if (recvall(s1, (char*)header, 3*sizeof(unsigned int)) != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	val = header[0];

	// Special number to indicate a full image.
	if (val == UINT_MAX)
	{
		// Full image data (with static compression).

		// Quick checks...
		if (((int)header[1] < 0)||((int)header[2] < 0))
		{
			printf("Bad compression or transmission error.\n");
			return EXIT_FAILURE;
		}

		mat = cvCreateMat(header[1], header[2], CV_8UC1);
		if (mat == NULL)
		{
			printf("cvCreateMat() failed.\n");
			return EXIT_FAILURE;
		}

		if (recvall(s1, (char*)mat->data.ptr, header[1]*header[2]) != EXIT_SUCCESS)
		{
			cvReleaseMat(&mat);
			return EXIT_FAILURE;
		}

		imagenew = cvDecodeImage(mat, CV_LOAD_IMAGE_COLOR);
		if (imagenew == NULL)
		{
			printf("cvDecodeImage() failed.\n");
			cvReleaseMat(&mat);
			return EXIT_FAILURE;
		}
		cvReleaseImage(&image);
		image = imagenew;

		cvReleaseMat(&mat);

		// Resolution changed by server.
		if ((image->width != videoimgwidth)||(image->height != videoimgheight))
		{
			videoimgwidth = image->width;
			videoimgheight = image->height;

			if (bDynamicWindowResizing) cvResizeWindow("Client", videoimgwidth, videoimgheight);

			databufnew = (char*)calloc(image->imageSize+3*sizeof(unsigned int), sizeof(char));
			if (!databufnew)	
			{
				printf("realloc() failed.\n");
				return EXIT_FAILURE;
			}
			free(databuf);
			databuf = databufnew;
		}
	}
	else
	{
		// Partial image data (dynamic time compression) or full image data without compression.

		// Quick checks...
		if (((int)header[1] < 0)||((int)header[1] > 4096)||((int)header[2] < 0)||((int)header[2] > 4096)||(val > 3*4096*4096+3*sizeof(unsigned int)))
		{
			printf("Unable to set desired video resolution or transmission error.\n");
			return EXIT_FAILURE;
		}

		videoimgwidth = header[1];
		videoimgheight = header[2];

		// Resolution changed by server.
		if ((image->width != videoimgwidth)||(image->height != videoimgheight))
		{
			imagenew = cvCreateImage(cvSize(videoimgwidth, videoimgheight), IPL_DEPTH_8U, 3);
			if (imagenew == NULL)
			{
				printf("cvCreateImage() failed.\n");
				return EXIT_FAILURE;
			}
			cvReleaseImage(&image);
			image = imagenew;

			if (bDynamicWindowResizing) cvResizeWindow("Client", videoimgwidth, videoimgheight);

			databufnew = (char*)calloc(image->imageSize+3*sizeof(unsigned int), sizeof(char));
			if (!databufnew)	
			{
				printf("realloc() failed.\n");
				return EXIT_FAILURE;
			}
			free(databuf);
			databuf = databufnew;
		}

		nbBytes = val-3*sizeof(unsigned int);
		if (nbBytes > image->imageSize)
		{
			printf("Bad compression or transmission error.\n");
			return EXIT_FAILURE;
		}
		if (nbBytes > 0)
		{
			if (recvall(s1, databuf, nbBytes) != EXIT_SUCCESS)
			{
				return EXIT_FAILURE;
			}
			if (nbBytes == image->imageSize)
			{
				// Full image data without compression.
				memcpy(image->imageData, databuf, image->imageSize);
			}
			else
			{
				// Partial image data (dynamic time compression).
				i = nbBytes;
				while (i -= 7) // 7 for sizeof(unsigned int)+3*sizeof(char).
				{
					// Blue index value of the pixel.
					memcpy((char*)&val, databuf+i, sizeof(unsigned int)); 
					// Check if index is valid.
					if ((val < 0)||(val > image->imageSize-3*sizeof(char))) 
					{
						printf("Bad compression or transmission error.\n");
						return EXIT_FAILURE;
					}
					// BGR values.
					memcpy(image->imageData+val, databuf+i+sizeof(unsigned int), 3*sizeof(char));
				}
			}
		}
	}

	return EXIT_SUCCESS;
}

void CleanUp(void)
{
	if (bUDP) releaseudpcli(s1); else releasetcpcli(s1);
	free(databuf);
	cvReleaseImage(&image);
	cvDestroyWindow("Client");
}

#if defined(_WIN32) && !defined(_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	int argc = __argc;
	char** argv = __argv;
#else
int main(int argc, char* argv[])
{
#endif // defined(_WIN32) && !defined(_DEBUG)
	int c = 0;

	INIT_DEBUG;

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	LoadConfig();

	cvNamedWindow("Client", CV_WINDOW_KEEPRATIO);
	cvMoveWindow("Client", 0, 0);
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 1.0, 4, 8);

	cvResizeWindow("Client", videoimgwidth, videoimgheight);
	cvWaitKey(3000);

	image = cvCreateImage(cvSize(videoimgwidth, videoimgheight), IPL_DEPTH_8U, 3);
	if (image == NULL)
	{
		printf("cvCreateImage() failed.\n");
		cvDestroyWindow("Client");
		return EXIT_FAILURE;
	}

	databuf = (char*)calloc(image->imageSize+3*sizeof(unsigned int), sizeof(char));
	if (!databuf)	
	{
		printf("calloc() failed.\n");
		cvReleaseImage(&image);
		cvDestroyWindow("Client");
		return EXIT_FAILURE;
	}

	if (
		(bUDP&&(initudpcli(&s1, cli1address, cli1port) != EXIT_SUCCESS))||
		((!bUDP)&&(inittcpcli(&s1, cli1address, cli1port) != EXIT_SUCCESS))
		)
	{
		free(databuf);
		cvReleaseImage(&image);
		cvDestroyWindow("Client");
		return EXIT_FAILURE;
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
		FD_SET(s1, &sock_set);

		iResult = select((int)s1+1, &sock_set, NULL, NULL, &tv);

		// Remove the socket from the set.
		// No need to use FD_ISSET() here, as we only have one socket the return value of select() is 
		// sufficient to know what happened.
		FD_CLR(s1, &sock_set); 

		switch (iResult)
		{
		case SOCKET_ERROR:
			CleanUp();
			return EXIT_FAILURE;
		case 0:
			// The timeout on select() occured.
			break;
		default:
			if (recvdecode() != EXIT_SUCCESS)
			{
				CleanUp();
				return EXIT_FAILURE;
			}

			cvShowImage("Client", image);

			c = cvWaitKey(captureperiod);
			if ((char)c == 27)
			{
				CleanUp();
				return EXIT_SUCCESS;
			}
			break;
		}
	}

	if (
		(bUDP&&(releaseudpcli(s1) != EXIT_SUCCESS))||
		((!bUDP)&&(releasetcpcli(s1) != EXIT_SUCCESS))
		)
	{
		free(databuf);
		cvReleaseImage(&image);
		cvDestroyWindow("Client");
		return EXIT_FAILURE;
	}

	free(databuf);
	cvReleaseImage(&image);
	cvDestroyWindow("Client");

	return EXIT_SUCCESS;
}
