/* Applied Video Sequence Analysis - Escuela Politecnica Superior - Universidad Autonoma de Madrid
 *
 *	Starter code for Task 3.1a of the assignment "Lab3 - Kalman Filtering for object tracking"
 *
 *	This code has been tested using Ubuntu 18.04, OpenCV 3.4.4 & Eclipse 2019-12
 *
 * Author: Juan C. SanMiguel (juancarlos.sanmiguel@uam.es)
 * Date: March 2022
 */
//includes
#include <opencv2/opencv.hpp> 	//opencv libraries
#include <opencv2/video/background_segm.hpp>
#include "opencv2/core.hpp"
//Header ShowManyImages
#include "ShowManyImages.hpp"

//include for blob-related functions
#include "blobs.hpp"

//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;

// set minimum size for blob
#define MIN_WIDTH 8
#define MIN_HEIGHT 8

//main function
int main(int argc, char ** argv)
{
	int count=0; // frame counter
	Mat frame;   // initialize frame
	Mat b_frame; // initialize frame for displaying blobs
	Mat f_frame; // initialize frame for marking blob and displaying coordinates

	// initialize variables for gray image, foreground mask and mdified foreground mask
	cv::Mat gray = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
	cv::Mat fgmask = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
	cv::Mat opened_fgmask = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);

	// initialize x and y for points of the center of the blob
	int x = 0;
	int y = 0;

	std::vector<cv::Point> points; // point for the center of the biggest blob
	std::vector<cvBlob> bloblist; // list for blobs
	std::vector<cvBlob> bloblistFiltered; // store the biggest blob per frame

	// frame of the video sequence
	std::string inputvideo = "/home/zsofi/AVSA_Lab3_datasets/dataset_lab3/lab3.1/singleball.mp4"; 	// path for the video to process

	//alternatively, the videofile can be passed via arguments of the executable
	if (argc == 3) inputvideo = argv[1];
	VideoCapture cap(inputvideo);							// reader to grab frames from video

	//check if videofile exists
	if (!cap.isOpened())
		throw std::runtime_error("Could not open video file " + inputvideo); //throw error if not possible to read videofile


	 // Create the bgsubtractor and set threshold variable and history value
	 Ptr<BackgroundSubtractor> bgsubtractor = cv::createBackgroundSubtractorMOG2(50,  36.0, true);


	//main loop
	for (;;) {

		std::cout << "FRAME " << std::setfill('0') << std::setw(3) << ++count << std::endl; //print frame number

		//get frame & check if we achieved the end of the videofile (e.g. frame.data is empty)
		cap >> frame;
		if (!frame.data)
			break;

		//do measurement extraction
		//PLACE YOUR CODE HERE


		cvtColor(frame, gray, COLOR_BGR2GRAY); // convert the frame to grayscale
		bgsubtractor->apply(gray, fgmask, 0.001 ); // apply foreground detection set learning rate in last parametera

		cv::morphologyEx(fgmask,opened_fgmask,MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(3,3))); // apply morphological opening

		int connectivity = 8; // 4 or 8
		extractBlobs(opened_fgmask, bloblist, connectivity); // extract blobs and store them in bloblist
		removeSmallBlobs(bloblist, bloblist, MIN_WIDTH, MIN_HEIGHT, opened_fgmask); // remove small blobs
		getBiggestBlob(bloblist, bloblistFiltered); // select the biggest blob in the frame

		// If there is a biggest blob in the frame, store the center in the points list
		if (bloblistFiltered.size()==1){
				x = bloblistFiltered[0].x + int(bloblistFiltered[0].w/2);
				y = bloblistFiltered[0].y +int(bloblistFiltered[0].h/2);
				points.push_back(cv::Point(x,y));
		}

		//...
		
		//display frame (YOU MAY ADD YOUR OWN VISUALIZATION FOR MEASUREMENTS, AND THE STAGES IMPLEMENTED)
		string title = "Lab3.1 result"; // title for the displayed frame

		// copy frame to display blob and the track
		frame.copyTo(b_frame);
		frame.copyTo(f_frame);

		// Create the texts for the displayed frames
		putText(frame,"Frame " + std::to_string(count), cvPoint(30,30),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
		putText(opened_fgmask,"FG mask  " + std::to_string(count), cvPoint(30,30),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
		putText(b_frame,"Blobs " + std::to_string(count), cvPoint(30,30),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);

		for (unsigned int i = 0; i < points.size(); i++){
			drawMarker(f_frame, points[i], cvScalar(255,0,0), MARKER_TILTED_CROSS, 20,2); //display measurement as a marker
		}
		putText(f_frame,"Measured place: x = " + std::to_string(x) + " y = " +  std::to_string(y), cvPoint(30,30),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);

		// Display the 4 frames: basic frame, foreground mask, detected blobs on the frame, biggest blob with the measured center
		ShowManyImages(title, int(4), frame, opened_fgmask, paintBlobImage(b_frame,bloblist, false),  paintBlobImage(f_frame,bloblistFiltered, false));

		std::ostringstream str;
		str << std::setfill('0') << std::setw(3) << count;
		// imshow("Frame ",frame);

		//cancel execution by pressing "ESC"
		if( (char)waitKey(100) == 27)
			break;

	}

	printf("Finished program.");
	destroyAllWindows(); 	// closes all the windows
	return 0;
}
