/*
 * kf.hpp
 *
 *  Created on: Apr 24, 2023
 *      Author: zsofi
 */

#ifndef SRC_KF_HPP_
#define SRC_KF_HPP_


#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace kf{
class Kalman_Filt {
public:
	Kalman_Filt(int model);
	virtual ~Kalman_Filt(){};
	void update(int count, cv::Point meas );
	std::vector<Point> getMeasPoints(){
		return meas_points;
	}

	std::vector<Point> getEstimatedPoints(){
		return estimated_points;
	}
	cv::Point getEstimatedPt(){
		return estimatedPt;
	}

private:
	cv::KalmanFilter _KF;

	std::vector<Point> meas_points;
	std::vector<Point> estimated_points;

	int ERROR_COV_POST = 1e2; // Related to P: how much we trust the prediction at the begining, high value-> we dont trust our model
	int MEAS_NOISE_COV = 30;  // Related to R: how noisy is our meassurement. Small value -> we dont trust predictiong and trust measurement
	int NOISECOV_POS   = 3;  //Guess on the object movement, it is not going to move more than these amount of pixels per frame
	int NOISECOV_VEL   = 0.5;  //Guess on the object velocity variance
	int NOISECOV_ACC   = 0.2;   //Guess on the object acceleration variance
	int SizeMea;
	int SizeSta;
	int controlParams;
	int index_x;
	int index_y;

	cv::Mat state;
	cv::Mat measurements;
	cv::Mat corrected;
	bool firstMea = true;
	cv::Point estimatedPt;

};
}


#endif /* SRC_KF_HPP_ */




