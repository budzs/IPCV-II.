/*
 * kf.cpp
 *
 *  Created on: Apr 24, 2023
 *      Author: zsofi
 */


#include "kf.hpp"

#include <opencv2/opencv.hpp> 	//opencv libraries
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/cvdef.h"
#include <stdio.h>




//namespaces
using namespace cv;
//avoid using 'cv' to declare OpenCV functions and variables (cv::Mat or Mat)
using namespace std;

using namespace kf;

Kalman_Filt::Kalman_Filt(int model) {
	// TODO Auto-generated constructor stub
	//=============================================================
		// Kalman filter initialization
		//===========================================

		if (model == 1) {
			SizeMea = 2;
			SizeSta = 4;
			controlParams = 0;
			index_x = 0;
			index_y = 2;

		} else if (model == 2) {
			SizeMea = 2;
			SizeSta = 6;
			controlParams = 0;
			index_x = 0;
			index_y = 3;
		}

		printf("Creation of Kalman object.\n");
		Kalman_Filt::_KF= cv::KalmanFilter(SizeSta, SizeMea, controlParams);// 2 dynamiParams state variables (position and velocity) and 2 measureParams (x,y position of the ball)

		//Initialization of the Matrixes



		Mat measurement = Mat::zeros(SizeMea, 1, CV_32F);
		setIdentity(_KF.measurementNoiseCov, Scalar::all(MEAS_NOISE_COV)); //Matrix R
		setIdentity(_KF.errorCovPost, Scalar::all(ERROR_COV_POST)); //Matrix Pk=0

		if (model == 1) {
			//Constant Velocity Model



			_KF.transitionMatrix = (Mat_<float>(SizeSta, SizeSta) << 1, 1, 0, 0, // A: Transition Matrix
			                                                        0, 1, 0, 0,
																	0, 0, 1, 1,
																	0, 0, 0, 1);

			_KF.measurementMatrix = (Mat_<float>(SizeMea, SizeSta) << 1, 0, 0, 0, // H  Observation Matrix
			                                                         0, 0, 1, 0);

			//Matrix Q
			cv::setIdentity(_KF.processNoiseCov, cv::Scalar(NOISECOV_VEL));
			_KF.processNoiseCov.at<float>(0) = NOISECOV_POS;
			_KF.processNoiseCov.at<float>(10) = NOISECOV_POS;



		} else if (model == 2) {
			//Constant Acceleration Model

			_KF.transitionMatrix = // A: Transition Matrix
					(Mat_<float>(SizeSta, SizeSta) << 1, 1, 0.5, 0, 0, 0,
					                                  0, 1,   1, 0, 0, 0,
													  0, 0,   1, 0, 0, 0,
													  0, 0,   0, 1, 1, 0.5,
													  0, 0,   0, 0, 1, 1,
													  0, 0,   0, 0, 0, 1);

			_KF.measurementMatrix =  // H  Observation Matrix
					(Mat_<float>(SizeMea, SizeSta) << 1, 0, 0, 0, 0, 0,
					                                  0, 0, 0, 1, 0, 0);

//			_KF.processNoiseCov = //Matrix Q
//					(Mat_<float>(SizeSta, SizeSta) << 25, 0, 0, 0, 0, 0,
//					                                  0, 10, 0, 0, 0, 0,
//													  0, 0,  1, 0, 0, 0,
//													  0, 0, 0, 25, 0, 0,
//													  0, 0, 0, 0, 10, 0,
//													  0, 0, 0, 0, 0, 1);
			cv::setIdentity(_KF.processNoiseCov, cv::Scalar(NOISECOV_ACC));
			_KF.processNoiseCov.at<float>(0) = NOISECOV_POS;
			_KF.processNoiseCov.at<float>(7) = NOISECOV_VEL;
			_KF.processNoiseCov.at<float>(21) = NOISECOV_POS;
			_KF.processNoiseCov.at<float>(28) = NOISECOV_VEL;
		}
		std::cout<< model<< endl;

		state = cv::Mat(SizeSta, 1, CV_32F);       // Constant Speed model [x,vx,y,vy]										 // Constante Acceleration Model [x,vx,ax,y,vy,ay]
		measurements = cv::Mat(SizeMea, 1, CV_32F); // [zx,zy]
}

void Kalman_Filt::update(int count, cv::Point meas ){
	measurements.at<float>(0) = float(meas.x); // measured x position
	measurements.at<float>(1) = float(meas.y); // measured y position


	if (meas.x != -10 && meas.y != -100) { //There is a measurement

				std::cout << "Measured  State: (" << meas.x <<  "," << meas.y << ")" << endl;
				meas_points.push_back(meas);

				if (firstMea) {

					//Initialization with current position and velocity = 0
					_KF.statePre = Mat::zeros(SizeSta, 1, CV_32F);
					_KF.statePost = Mat::zeros(SizeSta, 1, CV_32F);
					_KF.statePre.at<float>(index_x) = meas.x;
					_KF.statePre.at<float>(index_y) = meas.y;
					_KF.statePost.at<float>(index_x) = meas.x;
					_KF.statePost.at<float>(index_y) = meas.y;
					std::cout<<measurements.size()<<endl;
					corrected = _KF.correct(measurements);


					estimatedPt.x = corrected.at<float>(index_x);
					estimatedPt.y = corrected.at<float>(index_y);
					estimated_points.push_back(estimatedPt);
					std::cout << std::setprecision(3) << "Predicted State: No prediction, this is the first measurement!" << endl;
					std::cout << "Corrected State: (" << estimatedPt.x <<  "," << estimatedPt.y << ")" << endl;
					firstMea = false;

				}else {
					state = _KF.predict();
					corrected = _KF.correct(measurements);
					estimatedPt.x = corrected.at<float>(index_x);
					estimatedPt.y = corrected.at<float>(index_y);
					estimated_points.push_back(estimatedPt);
					std::cout<< std::setprecision(3) << "Predicted State: (" << state.at<float>(index_x) <<  "," << state.at<float>(index_y) << ")" << endl;
					std::cout << "Corrected State: (" << estimatedPt.x <<  "," << estimatedPt.y << ")" << endl;
				}

			}

			else { //No measurement available

				std::cout << "Measured  State: No measurement avaliable" << endl;

				if (firstMea) {

					estimatedPt.x =  meas.x;
					estimatedPt.y =  meas.y;

				}else{// Use the prediction

					state = _KF.predict();
					estimatedPt.x = state.at<float>(index_x);
					estimatedPt.y = state.at<float>(index_y);
					estimated_points.push_back(estimatedPt);

					std::cout << "Predicted State: (" << state.at<float>(index_x) <<  "," << state.at<float>(index_y) << ")" << endl;
					std::cout << "Corrected State: No correction possible!" << endl;
				}
			}



}




