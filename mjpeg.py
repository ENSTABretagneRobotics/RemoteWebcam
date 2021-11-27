import cv2

capture = cv2.VideoCapture("http://127.0.0.1:4014/?action=stream?dummy=param.mjpg")
try:
	while True:
		ret, frame = capture.read()
		if ret > 0:
		    cv2.imshow("Python MJPEG Client", frame)
		cv2.waitKey(10)
except KeyboardInterrupt:
	print("Interrupted\n")
