/* Applied Video Analysis of Sequences (AVSA)
 *
 *	LAB2: Blob detection & classification
 *	Lab2.0: Sample Opencv project
 *
 *
 * Authors: José M. Martínez (josem.martinez@uam.es), Paula Moral (paula.moral@uam.es), Juan C. San Miguel (juancarlos.sanmiguel@uam.es)
 */

#include "blobs.hpp"

/**
 *	Draws blobs with different rectangles on the image 'frame'. All the input arguments must be
 *  initialized when using this function.
 *
 * \param frame Input image
 * \param pBlobList List to store the blobs found
 * \param labelled - true write label and color bb, false does not wirite label nor color bb
 *
 * \return Image containing the draw blobs. If no blobs have to be painted
 *  or arguments are wrong, the function returns a copy of the original "frame".
 *
 */
 Mat paintBlobImage(cv::Mat frame, std::vector<cvBlob> bloblist, bool labelled)
{
	cv::Mat blobImage;
	//check input conditions and return original if any is not satisfied
	//...
	frame.copyTo(blobImage);

	//required variables to paint
	//...

	//paint each blob of the list
	for(unsigned int i = 0; i < bloblist.size(); i++)
	{
		cvBlob blob = bloblist[i]; //get ith blob
		//...
		Scalar color;
		std::string label="";
		switch(blob.label){
		case PERSON:
			color = Scalar(255,0,0);
			label="PERSON";
			break;
		case CAR:
					color = Scalar(0,255,0);
					label="CAR";
					break;
		case OBJECT:
					color = Scalar(0,0,255);
					label="OBJECT";
					break;
		default:
			color = Scalar(255, 255, 255);
			label="UNKOWN";
		}

		Point p1 = Point(blob.x, blob.y);
		Point p2 = Point(blob.x+blob.w, blob.y+blob.h);

		rectangle(blobImage, p1, p2, color, 1, 8, 0);
		if (labelled)
			{
			rectangle(blobImage, p1, p2, color, 1, 8, 0);
			putText(blobImage, label, p1, FONT_HERSHEY_SIMPLEX, 0.5, color);
			}
			else
				rectangle(blobImage, p1, p2, Scalar(255, 255, 255), 1, 8, 0);
	}

	//destroy all resources (if required)
	//...

	//return the image to show
	return blobImage;
}


/**
 *	Blob extraction from 1-channel image (binary). The extraction is performed based
 *	on the analysis of the connected components. All the input arguments must be 
 *  initialized when using this function.
 *
 * \param fgmask Foreground/Background segmentation mask (1-channel binary image) 
 * \param bloblist List with found blobs
 *
 * \return Operation code (negative if not succesfull operation) 
 */
int extractBlobs(cv::Mat fgmask, std::vector<cvBlob> &bloblist, int connectivity)
{	
	//check input conditions and return -1 if any is not satisfied
	//...		

	//required variables for connected component analysis 
	//...
	Mat aux; // image to be updated each time a blob is detected (blob cleared)
	fgmask.convertTo(aux,CV_8UC1);
	
	//clear blob list (to fill with this function)
	bloblist.clear();
			
	//Connected component analysis

	int blobId = 1;
	for (int i = 0; i < aux.rows; i++){
		    for (int j = 0; j < aux.cols; j++){

		    	if (aux.at<uchar>(i,j) == 255){

		    		// The function cv::floodFill fills a connected component starting from the seed point with the specified color.
		    		cv::Rect rect;
		    		// floodFill( InputOutputArray image, Point seedPoint, Scalar newVal, CV_OUT Rect* rect = 0, Scalar loDiff = Scalar(), Scalar upDiff = Scalar(), int flags = 4 );
		    		// connectivity : The default value of 4 means that only the four nearest neighbor pixels (those that share an edge) are considered

		    		cv::floodFill(aux, Point(j,i), 64, &rect,0,0, connectivity);

		    		//  creation of the blob
		    		cvBlob blob=initBlob(blobId, rect.x, rect.y, rect.width, rect.height);
		    		bloblist.push_back(blob);

		    		blobId = blobId + 1;

		    	}
		    }
		}

		
	
	// void creation of a unqie blob in the center
//		cvBlob blob=initBlob(1, fgmask.cols/2, fgmask.rows/2, fgmask.cols/4, fgmask.rows/4);
//		bloblist.push_back(blob);

//	std::cout << bkg << " " << fg << " " << sh <<" " << fill << " " << unknown << " "<< bkg+fg+sh+unknown  << " " << fgmask.rows*fgmask.cols << std::endl;
//	std::cout << blob_id << " " << small_blobs << std::endl;

	//destroy all resources
	//...

	//return OK code
	return 1;
}


int removeSmallBlobs(std::vector<cvBlob> bloblist_in, std::vector<cvBlob> &bloblist_out, int min_width, int min_height, Mat frame)
{
	//check input conditions and return -1 if any is not satisfied

	//required variables
	//...

	//clear blob list (to fill with this function)
	bloblist_out.clear();
	//std::cout<< frame.size().height << "  "<< frame.size().width << std::endl;


	for(unsigned int i = 0; i < bloblist_in.size(); i++)
	{
		cvBlob blob_in = bloblist_in[i]; //get ith blob

		// ...
		int size = blob_in.h * blob_in.w;
		int threshold_min = min_width * min_height;
		int threshold_max = 0.9*(frame.size().height * frame.size().width);
		if(size >= threshold_min && size <= threshold_max){

			bloblist_out.push_back(blob_in);
		}

		//bloblist_out.push_back(blob_in); // void implementation (does not remove)

	}
	//destroy all resources
	//...
	
	//return OK code
	return 1;
}



 /**
  *	Blob classification between the available classes in 'Blob.hpp' (see CLASS typedef). All the input arguments must be
  *  initialized when using this function.
  *
  * \param frame Input image
  * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
  * \param bloblist List with found blobs
  *
  * \return Operation code (negative if not succesfull operation)
  */

 // ASPECT RATIO MODELS
#define MEAN_PERSON 0.3950
#define STD_PERSON 0.1887

#define MEAN_CAR 1.4736
#define STD_CAR 0.2329

#define MEAN_OBJECT 1.2111
#define STD_OBJECT 0.4470

// end ASPECT RATIO MODELS

// distances
float ED(float val1, float val2)
{
	return sqrt(pow(val1-val2,2));
}

float WED(float val1, float val2, float std)
{
	return sqrt(pow(val1-val2,2)/pow(std,2));
}
//end distances
 int classifyBlobs(std::vector<cvBlob> &bloblist)
 {
 	//check input conditions and return -1 if any is not satisfied
 	//...

 	//required variables for classification
 	//...

 	//classify each blob of the list
 	for(unsigned int i = 0; i < bloblist.size(); i++)
 	{
 		cvBlob blob = bloblist[i]; //get i-th blob
 		//...

//#define MEAN_PERSON 0.3950
//#define STD_PERSON 0.1887
//
//#define MEAN_CAR 1.4736
//#define STD_CAR 0.2329
//
//#define MEAN_OBJECT 1.2111
//#define STD_OBJECT 0.4470

 		float aspect_ratio = blob.w / blob.h;

 		// Calculate the Weigthed euclidean distance from the  ASPECT RATIO MODELS
 		float from_people =  ED( MEAN_PERSON,  aspect_ratio); //,  STD_PERSON); //sqrt((pow((MEAN_PERSON - aspect_ratio), 2)/STD_PERSON));
 		float from_car = ED( MEAN_CAR,  aspect_ratio); //,  STD_CAR);//sqrt((pow((MEAN_CAR - aspect_ratio), 2)/STD_CAR));
 		float from_object = ED( MEAN_OBJECT,  aspect_ratio); //,  STD_OBJECT);//sqrt((pow((MEAN_OBJECT - aspect_ratio), 2)/STD_OBJECT));

 		// std::cout<< from_people<< "  " <<from_car << "  " << from_object << std::endl;
 		int multiplier = 3;

 		// Find the smallest distance to decide the label
 		if ((from_people <= from_car && from_people <= from_object)&& (from_people <= multiplier*STD_PERSON)){
 			blob.label = PERSON;
 		}
 		else if ((from_car <= from_people && from_car <= from_object)&& (from_car <= multiplier*STD_CAR)){
 			blob.label = CAR;
 		}
 		else if (from_object <= multiplier*STD_OBJECT) {
 			blob.label = OBJECT;
 		}
 		else{
 			blob.label = UNKNOWN;
 		}


 		// Modify the blob with the label
 		bloblist[i] = blob;



 		// void implementation (does not change label -at creation UNKNOWN-)
 	}

 	//destroy all resources
 	//...

 	//return OK code
 	return 1;
 }

//stationary blob extraction function
 /**
  *	Stationary FG detection
  *
  * \param fgmask Foreground/Background segmentation mask (1-channel binary image)
  * \param fgmask_history Foreground history counter image (1-channel integer image)
  * \param sfgmask Foreground/Background segmentation mask (1-channel binary image)
  *
  * \return Operation code (negative if not succesfull operation)
  *
  *
  * Based on: Stationary foreground detection for video-surveillance based on foreground and motion history images, D.Ortego, J.C.SanMiguel, AVSS2013
  *
  */

#define FPS 25 //check in video - not really critical
#define SECS_STATIONARY 1 // to set
#define I_COST 1// to set // increment cost for stationarity detection
#define D_COST 15 // to set // decrement cost for stationarity detection
#define STAT_TH 0.8 // to set

 int extractStationaryFG (Mat fgmask, Mat &fgmask_history, Mat &sfgmask)
 {

	 int numframes4static=(int)(FPS*SECS_STATIONARY);

	 // cv::Mat foregroundHistoryImage = cv::Mat::zeros(Size(fgmask.cols, fgmask.rows), CV_32FC1);

	 // update fgmask_counter
	 for (int i=0; i<fgmask.rows;i++)
		 for(int j=0; j<fgmask.cols;j++)
		 {
			// ...
			 //fgmask_history.at<float>(i,j) = 0; // void implementation (no history)

			 if (fgmask.at<uchar>(i,j) == 255){

				 fgmask_history.at<uchar>(i,j) = fgmask_history.at<uchar>(i,j) +  I_COST ;//* fgmask.at<uchar>(i,j);// +


			 }
			 if(fgmask.at<uchar>(i,j) == 0){
				 fgmask_history.at<uchar>(i,j) = fgmask_history.at<uchar>(i,j) -  D_COST ; //* fgmask.at<uchar>(i,j); check 0
				 if(fgmask_history.at<uchar>(i,j) < 0){
					 fgmask_history.at<uchar>(i,j) = 0 ;
				 }
			 }


		 }//for

	// update sfgmask
	for (int i=0; i<fgmask.rows;i++)
		 for(int j=0; j<fgmask.cols;j++)
			 {
			 	 // ...
				 //sfgmask.at<uchar>(i,j)=0;// void implementation (no stationary fg)
			 // But this is (11) on the paper, not (12)

			 fgmask_history.at<uchar>(i,j) = min(1,(fgmask_history.at<uchar>(i,j)/numframes4static));

			 if (fgmask_history.at<uchar>(i,j) >= STAT_TH){

			 				sfgmask.at<uchar>(i,j)=255;

			 			 }
			 			 else{

			 				sfgmask.at<uchar>(i,j)=0;

			 			 }
			 }
 return 1;
 }



