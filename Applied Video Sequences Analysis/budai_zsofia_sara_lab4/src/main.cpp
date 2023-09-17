/* Applied Video Sequence Analysis - Escuela Politecnica Superior - Universidad Autonoma de Madrid
 *
 *	This source code belongs to the template (sample program)
 *	provided for the assignment LAB 4 "Histogram-based tracking"
 *
 *	This code has been tested using:
 *	- Operative System: Ubuntu 18.04
 *	- OpenCV version: 3.4.4
 *	- Eclipse version: 2019-12
 *
 * Author: Juan C. SanMiguel (juancarlos.sanmiguel@uam.es)
 * Date: April 2020
 */
//includes
#include <stdio.h> 								//Standard I/O library
#include <numeric>								//For std::accumulate function
#include <string> 								//For std::to_string function
#include <opencv2/opencv.hpp>					//opencv libraries
#include "utils.hpp" 							//for functions readGroundTruthFile & estimateTrackingPerformance

//namespaces
using namespace cv;
using namespace std;

//main function
int main(int argc, char ** argv)
{
	//PLEASE CHANGE 'dataset_path' & 'output_path' ACCORDING TO YOUR PROJECT
	std::string dataset_path = "/home/zsofi/Downloads/AVSA_lab4_datasets/datasets/";									//dataset location.
	std::string output_path = "/home/zsofi/eclipse-workspace/T4.1/outvideos/";									//location to save output videos

	// dataset paths
	std::string sequences[] = {"bolt1"};									//test data for lab4.1, 4.3 & 4.5
	//						   "sphere","car1",								//test data for lab4.2
	//						   "ball2","basketball",						//test data for lab4.4
	//						   "bag","ball","road",};						//test data for lab4.6
	std::string image_path = "%08d.jpg"; 									//format of frames. DO NOT CHANGE
	std::string groundtruth_file = "groundtruth.txt"; 						//file for ground truth data. DO NOT CHANGE
	int NumSeq = sizeof(sequences)/sizeof(sequences[0]);					//number of sequences

	// number of candidates: numCand = 1 -> 9 candidates , = 1 -> 25 candidates ...
	int numCand = 10;
	// how many steps per bounding box size
	int step = 20;

	//Loop for all sequence of each category
	for (int s=0; s<NumSeq; s++ )
	{
		Mat frame;										//current Frame
		int frame_idx=0;								//index of current Frame
		std::vector<Rect> list_bbox_est, list_bbox_gt;	//estimated & groundtruth bounding boxes
		std::vector<double> procTimes;					//vector to accumulate processing times

		std::string inputvideo = dataset_path + "/" + sequences[s] + "/img/" + image_path; //path of videofile. DO NOT CHANGE

		cv:: VideoCapture cap((inputvideo));	// reader to grab frames from videofile

		//check if videofile exists
		if (!cap.isOpened())
			throw std::runtime_error("Could not open video file " + inputvideo); //error if not possible to read videofile

		// Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
		cv::Size frame_size(cap.get(cv::CAP_PROP_FRAME_WIDTH),cap.get(cv::CAP_PROP_FRAME_HEIGHT));//cv::Size frame_size(700,460);
		VideoWriter outputvideo(output_path+"outvid_" + sequences[s]+".avi",CV_FOURCC('X','V','I','D'),10, frame_size);	//xvid compression (cannot be changed in OpenCV)

		//Read ground truth file and store bounding boxes
		std::string inputGroundtruth = dataset_path + "/" + sequences[s] + "/" + groundtruth_file;//path of groundtruth file. DO NOT CHANGE
		list_bbox_gt = readGroundTruthFile(inputGroundtruth); //read groundtruth bounding boxes
		int w = list_bbox_gt[10].width;
		int h = list_bbox_gt[10].height;
		cv::Rect pred = list_bbox_gt[0];

		//main loop for the sequence
		std::cout << "Displaying sequence at " << inputvideo << std::endl;
		std::cout << "  with groundtruth at " << inputGroundtruth << std::endl;
		int count = 0;
		for (;;) {
			//get frame & check if we achieved the end of the videofile (e.g. frame.data is empty)
			cap >> frame;
			if (!frame.data)
				break;

			//Time measurement
			double t = (double)getTickCount();
			frame_idx=cap.get(cv::CAP_PROP_POS_FRAMES);			//get the current frame

			////////////////////////////////////////////////////////////////////////////////////////////
			//DO TRACKING

			// ADD YOUR CODE HERE
			////////////////////////


			// Generate candidate locations from an initial location using the grid approach
			//described in the paper
			std::vector<Rect> candidates = createCandidates(numCand,pred, w, h, step, frame.size());

			// Compare object model and candidates using template matching in Gray Level
			cv::Rect objectModel = list_bbox_gt[count];
			std::vector<double> distances = compareCandidates(objectModel,candidates, frame);

			//Select the best candidate with the lowest template matching distance
			auto minDistIt = std::min_element(distances.begin(), distances.end());
			int ind = std::distance(distances.begin(), minDistIt);

			// Get the best candidate
			list_bbox_est.push_back(candidates[ind]);
			pred = candidates[ind];


			////////////////////////////////////////////////////////////////////////////////////////////

			//Time measurement
			procTimes.push_back(((double)getTickCount() - t)*1000. / cv::getTickFrequency());
			//std::cout << " processing time=" << procTimes[procTimes.size()-1] << " ms" << std::endl;

			// plot frame number & groundtruth bounding box for each frame
			putText(frame, std::to_string(frame_idx), cv::Point(10,15),FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255)); //text in red
			rectangle(frame, list_bbox_gt[frame_idx-1], Scalar(0, 255, 0));		//draw bounding box for groundtruth
			rectangle(frame, list_bbox_est[frame_idx-1], Scalar(255, 0, 255));	//draw bounding box (estimation)

			// Uncomment the following to draw all the candide bounding boxes
			/*
			for(unsigned int j = 0; j< candidates.size(); j++){
				rectangle(frame, candidates[j], Scalar(255, 0, 0), 1);	//draw bounding boxes (estimation)
			}*/

			//show & save data
			imshow("Tracking for "+sequences[s]+" (Green=GT, Red=Estimation)", frame);
			outputvideo.write(frame);//save frame to output video

			//exit if ESC key is pressed
			if(waitKey(30) == 27) break;
			count += 1;
		}

		//comparison groundtruth & estimation
		vector<float> trackPerf = estimateTrackingPerformance(list_bbox_gt, list_bbox_est);

		//print stats about processing time and tracking performance
		std::cout << "  Average processing time = " << std::accumulate( procTimes.begin(), procTimes.end(), 0.0) / procTimes.size() << " ms/frame" << std::endl;
		std::cout << "  Average tracking performance = " << std::accumulate( trackPerf.begin(), trackPerf.end(), 0.0) / trackPerf.size() << std::endl;

		//release all resources
		cap.release();			// close inputvideo
		outputvideo.release(); 	// close outputvideo
		destroyAllWindows(); 	// close all the windows
	}
	printf("Finished program.");
	return 0;
}
