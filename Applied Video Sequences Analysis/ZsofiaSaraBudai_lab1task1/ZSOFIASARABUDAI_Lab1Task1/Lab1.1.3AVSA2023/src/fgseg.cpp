/* Applied Video Sequence Analysis (AVSA)
 *
 *	LAB1.0: Background Subtraction - Unix version
 *	fgesg.cpp
 *
 * 	Authors: José M. Martínez (josem.martinez@uam.es), Paula Moral (paula.moral@uam.es) & Juan Carlos San Miguel (juancarlos.sanmiguel@uam.es)
 *	VPULab-UAM 2020
 */

#include <opencv2/opencv.hpp>
#include "fgseg.hpp"

using namespace fgseg;

//default constructor

bgs::bgs(double threshold, double alpha, int threshold_ghosts2, bool rgb) {
	_rgb = rgb;

	_threshold = threshold;

	//New parameters for the constructor
	_alpha = alpha;

	_threshold_ghosts2 = threshold_ghosts2;

}

//default destructor
bgs::~bgs(void) {
}

//method to initialize bkg (first frame - hot start)
void bgs::init_bkg(cv::Mat Frame) {

	if (!_rgb) {
		cvtColor(Frame, Frame, COLOR_BGR2GRAY); // to work with gray even if input is color
		counter = cv::Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1);
		_bkg = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		//ADD YOUR CODE HERE
		//...
		Frame.copyTo(_bkg);
		//...
	} else {
		// Initialize _bkg for rgb input
		//CV_<bit-depth>{U|S|F}C(<number_of_channels>)
		_bkg = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC3);

		// Initialize counter for suppression of ghosts
		counter = cv::Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC3);
		Frame.copyTo(_bkg);

	}

}

//method to perform BackGroundSubtraction
void bgs::bkgSubtraction(cv::Mat Frame) {

	if (!_rgb) {
		cvtColor(Frame, Frame, COLOR_BGR2GRAY); // to work with gray even if input is color
		Frame.copyTo(_frame);

		_diff = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		_bgsmask = Mat::zeros(Size(Frame.cols, Frame.rows), CV_8UC1); // void function for Lab1.0 - returns zero matrix
		//ADD YOUR CODE HERE
		//...

		// Calculating _bkg from the equation
		_bkg = (_alpha * _frame) + (1 - _alpha) * _bkg;

		// Calculating absolute difference between image and background pixels
		absdiff(_bkg, _frame, _diff);

		// If the previously calculated difference is bigger than the threshold the pixel belongs to foreground
		// Threshold for the binary mask
		threshold(_diff, _bgsmask, _threshold, 255, THRESH_BINARY);

		//Loop through every pixel
		for (int i = 0; i < _frame.rows; ++i) {
			for (int j = 0; j < _frame.cols; ++j) {

				// Counter increases every time the pixel is detected as foreground
				if (_bgsmask.at<uchar>(i, j) == 255) {
					counter.at<uchar>(i, j) = counter.at<uchar>(i, j) + 1;

					// Pixel becomes background if the counter reaches the threshold
					if (double(counter.at<uchar>(i, j)) >= _threshold_ghosts2) {
						_bkg.at<uchar>(i, j) = _frame.at<uchar>(i, j);
					}
				}
				// Counter resets every time the pixel is detected as background
				else if (_bgsmask.at<uchar>(i, j) == 0) {
					counter.at<uchar>(i, j) = 0;
				}
			}
		}

		//...
	} else {

		Frame.copyTo(_frame);

		_diff = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC3);
		_bgsmask = Mat::zeros(Size(Frame.cols,Frame.rows), CV_8UC3);

		// Calculating _bkg from the equation
		_bkg = (_alpha * _frame) + (1 - _alpha) * _bkg;

		// Calculating absolute difference between image and background pixels
		absdiff(_bkg, _frame, _diff);

		// If the previously calculated difference is bigger than the threshold the pixel belongs to foreground
		// Threshold for the binary mask
		threshold(_diff, _bgsmask, _threshold, 255, THRESH_BINARY);

		//Loop through every pixel
		for (int i = 0; i < _frame.rows; ++i) {
			for (int j = 0; j < _frame.cols; ++j) {

				// Loop through the 3 color channel
				for (int k = 0; k < 3; k++) {

					// Counter increases every time the pixel is detected as foreground
					if (_bgsmask.at<cv::Vec3b>(i, j)[k] == 255) {
						counter.at<cv::Vec3b>(i, j)[k] = double(
								counter.at<cv::Vec3b>(i, j)[k]) + 1;

						// Pixel becomes background if the counter reaches the threshold
						if (double(counter.at<cv::Vec3b>(i, j)[k])
								>= _threshold_ghosts2) {
							_bkg.at<cv::Vec3b>(i, j)[k] = _frame.at<cv::Vec3b>(
									i, j)[k];
						}
					}
					// Counter resets every time the pixel is detected as background
					else if (_bgsmask.at<cv::Vec3b>(i, j)[k] == 0) {
						counter.at<cv::Vec3b>(i, j)[k] = 0;
					}
				}
			}
		}
	}

}

//method to detect and remove shadows in the BGS mask to create FG mask
void bgs::removeShadows() {
	// init Shadow Mask (currently Shadow Detection not implemented)
	_bgsmask.copyTo(_shadowmask); // creates the mask (currently with bgs)

	//ADD YOUR CODE HERE
	//...
	absdiff(_bgsmask, _bgsmask, _shadowmask);
	//...

	absdiff(_bgsmask, _shadowmask, _fgmask); // eliminates shadows from bgsmask
}

//ADD ADDITIONAL FUNCTIONS HERE

