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
#include "kf.hpp"

//namespaces
using namespace cv; //avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;

// set minimum size for blob
#define MIN_WIDTH 50
#define MIN_HEIGHT 50

//main function
int main(int argc, char ** argv)
{
	int count=0; // frame counter
	bool stored_points = false;
	Mat frame;   // initialize frame
	Mat b_frame; // initialize frame for displaying blobs
	Mat f_frame; // initialize frame for marking blob and displaying coordinates
	Mat initFrame;
	Mat img_kalman;



	std::vector<cv::Point> measList; //variable where measurements will be stored
	std::string line;

	// initialize variables for gray image, foreground mask and mdified foreground mask
	cv::Mat gray = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
	cv::Mat fgmask = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);
	cv::Mat opened_fgmask = Mat::zeros(Size(frame.cols, frame.rows), CV_8UC1);

	// initialize x and y for points of the center of the blob
	int x = -100;
	int y = -100;

	std::vector<cv::Point> points; // point for the center of the biggest blob
	std::vector<cvBlob> bloblist; // list for blobs
	std::vector<cvBlob> bloblistFiltered; // store the biggest blob per frame

	// frame of the video sequence
	std::string inputvideo = "/home/zsofi/AVSA_Lab3_datasets/dataset_lab3/lab3.3/abandonedBox_600_1000_clip.mp4"; 	// path for the video to process

	//alternatively, the videofile can be passed via arguments of the executable
	if (argc == 3) inputvideo = argv[1];
	VideoCapture cap(inputvideo);							// reader to grab frames from video

	//check if videofile exists
	if (!cap.isOpened())
		throw std::runtime_error("Could not open video file " + inputvideo); //throw error if not possible to read videofile


	 // Create the bgsubtractor and set threshold variable and history value
	 Ptr<BackgroundSubtractor> bgsubtractor = cv::createBackgroundSubtractorMOG2(300,  20.0, true);

	 int model = 2;
	 kf::Kalman_Filt Kalman_Filt(model);


/*		//preload measurements from txt file
	std::ifstream ifile("/home/zsofi/AVSA_Lab3_datasets/dataset_lab3/lab3.1/meas_singleball.txt"); //filename with measurements (each line corresponds to X-Y coordinates of the measurement obtained for each frame)
		// auxiliary variable to read each line of file
	while (std::getline(ifile, line)) // read the current line
		{
			std::istringstream iss { line }; // construct a string stream from line

			// read the tokens from current line separated by comma
			std::vector<std::string> tokens; // here we store the tokens
			std::string token; // current token
			while (std::getline(iss, token, ' '))
				tokens.push_back(token); // add the token to the vector

			measList.push_back(
					cv::Point(std::stoi(tokens[0]), std::stoi(tokens[1])));
			//std::cout << "Processed point: " << std::stoi(tokens[0]) << " " << std::stoi(tokens[1]) << std::endl; //display read data
		}*/



	//main loop
	for (;;) {

		std::cout << "FRAME " << std::setfill('0') << std::setw(3) << ++count << std::endl; //print frame number

		//get frame & check if we achieved the end of the videofile (e.g. frame.data is empty)
		cap >> frame;
		if (!frame.data)
			break;

		if (count == 1) {

			frame.copyTo(initFrame);
			frame.copyTo(img_kalman);

		}

		//do measurement extraction
		//PLACE YOUR CODE HERE


		cvtColor(frame, gray, COLOR_BGR2GRAY); // convert the frame to grayscale
		float lr = 0.01;
		bgsubtractor->apply(gray, fgmask, lr ); // apply foreground detection set learning rate in last parametera

		cv::morphologyEx(fgmask,opened_fgmask,MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(4,4))); // apply morphological opening

		cv::morphologyEx(fgmask,opened_fgmask,MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(2,2))); // apply morphological opening
		cv::morphologyEx(fgmask,opened_fgmask,MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(4,4))); // apply morphological opening

		int connectivity = 4; // 4 or 8
		extractBlobs(opened_fgmask, bloblist, connectivity); // extract blobs and store them in bloblist
		removeSmallBlobs(bloblist, bloblistFiltered, MIN_WIDTH, MIN_HEIGHT, opened_fgmask); // remove small blobs
		getBiggestBlob(bloblistFiltered, bloblistFiltered); // select the biggest blob in the frame

		// If there is a biggest blob in the frame, store the center in the points list
		if (bloblistFiltered.size()==1){
				x = bloblistFiltered[0].x + int(bloblistFiltered[0].w/2);
				y = bloblistFiltered[0].y +int(bloblistFiltered[0].h/2);

		}
		else{
			x = -100;
			y = -100;
		}
		points.push_back(cv::Point(x,y));

		//...

		if (stored_points){
			Kalman_Filt.update(count, measList[count-1]);
		}else{
			Kalman_Filt.update(count, cv::Point(x,y));

		}




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



		std::ostringstream str;
		str << std::setfill('0') << std::setw(3) << count;
		// imshow("Frame ",frame);

		//-------------------------------------
		// Displaying figures
		//-------------------------------------

//		if (display_meassurements) {
//			//display frame (YOU MAY ADD YOUR OWN VISUALIZATION FOR MEASUREMENTS, AND THE STAGES IMPLEMENTED)
//			std::ostringstream str;
//			str << std::setfill('0') << std::setw(3) << count;
//			putText(frame, "Frame " + str.str(), cvPoint(30, 30),
//					FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 255), 1,
//					CV_AA);
//			drawMarker(frame, cv::Point(x,y), cvScalar(255, 0, 0), MARKER_CROSS, 20, 2); //display measurement
//			imshow("Frame ", frame);
//
//			//cancel execution by pressing "ESC"
//			if ((char) waitKey(100) == 27)
//				break;
//		}


		// SHOWING THE PREDICTED AN MEASSURED STATE OVER THE IMAGE
		if (count == 1) {
			putText(img_kalman, "Estimated z_k", cvPoint(30, 50),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(161, 67, 114), 1,CV_AA);
			putText(img_kalman, "Measurement x_k", cvPoint(30, 70),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(39, 184, 253), 1,CV_AA);
			if (model == 1) {
				putText(img_kalman, "Model 1: Constant Velocity", cvPoint(30, 30),FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(255, 255, 255), 1, CV_AA);
			} else if (model == 2) {
				putText(img_kalman, "Model 2: Constant Acceleration", cvPoint(30, 30),FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(255, 255, 255), 1, CV_AA);
			}
		}

		drawMarker(img_kalman, cv::Point(x,y), cvScalar(39, 184, 253), MARKER_CROSS, 8, 2); //display measurement
		drawMarker(img_kalman, Kalman_Filt.getEstimatedPt(), cvScalar(131, 37, 84), MARKER_SQUARE,
				8, 2); //display measurement


		imshow("Kalman Tracker ", img_kalman);

		// Display the 4 frames: basic frame, foreground mask, detected blobs on the frame, biggest blob with the measured center
		ShowManyImages(title, int(4), frame, opened_fgmask, paintBlobImage(b_frame,bloblist, false),  img_kalman);

		//cancel execution by pressing "ESC"
		if( (char)waitKey(50) == 27)
			break;

	}// THE END OF THE MAIN LOOP

	imwrite("/home/avsa/lab3output/savedmod.png", img_kalman);

		Mat meas_img;
		frame.copyTo(meas_img);

		putText(initFrame, "Estimated z_k", cvPoint(30, 50),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 200, 0), 1,CV_AA);
		putText(initFrame, "Measurement x_k", cvPoint(30, 70),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(39, 184, 253), 1,CV_AA);
		if (model == 1) {
			putText(initFrame, "Model 1: Constant Velocity", cvPoint(30, 30),FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(255, 255, 255), 1, CV_AA);
		} else if (model == 2) {
			putText(initFrame, "Model 2: Constant Acceleration", cvPoint(30, 30),FONT_HERSHEY_COMPLEX_SMALL, 0.5, cvScalar(255, 255, 255), 1, CV_AA);
		}
		// Draw lines connecting points
		for (unsigned int i = 0; i < Kalman_Filt.getMeasPoints().size() - 1; i++) {
			cv::line( initFrame, Kalman_Filt.getMeasPoints()[i], Kalman_Filt.getMeasPoints()[i+1], Scalar(39, 184, 253), 1, LINE_AA, 0 );
		}
		for (unsigned int i = 0; i < Kalman_Filt.getEstimatedPoints().size() - 1; i++) {
			cv::line( initFrame, Kalman_Filt.getEstimatedPoints()[i], Kalman_Filt.getEstimatedPoints()[i+1], cvScalar(0, 200, 0), 1, LINE_AA, 0 );
		}


		    // Display image
		    imshow("Obtained trajectories", initFrame);
		    imwrite("/home/avsa/lab3output/trajectories_model2mod.png", initFrame);
		    waitKey(0);



	printf("Finished program.");
	destroyAllWindows(); 	// closes all the windows
	return 0;
}
