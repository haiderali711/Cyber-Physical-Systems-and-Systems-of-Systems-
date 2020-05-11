/*
 * Copyright (C) 2020  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
// Include the single-file, header-only middleware libcluon to create high-performance microservices
#include "cluon-complete.hpp"
// Include the OpenDLV Standard Message Set that contains messages that are usually exchanged for automotive or robotic applications
#include "opendlv-standard-message-set.hpp"
 
// Include the GUI and image processing header files from OpenCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
 


/********************************************
****************CONE CHECKING FUNCTION*******
********************************************/
bool checkConePresence(cv::Mat imageSegment) {

	//THE FUNCTIONS IMPLEMENTATION GOES HERE

	//RETURNING TRUE just int case for the sake of having a testing prototype with default TRUE
  	return true;
} 



//*****************MAIN**********************
 
int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};

    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ( (0 == commandlineArguments.count("cid")) ||
         (0 == commandlineArguments.count("name")) ||
         (0 == commandlineArguments.count("width")) ||
         (0 == commandlineArguments.count("height")) ) {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    }
    else {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};
 
 
        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid()) {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;
 
            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
 
            opendlv::proxy::GroundSteeringRequest gsr;
            std::mutex gsrMutex;
            auto onGroundSteeringRequest = [&gsr, &gsrMutex](cluon::data::Envelope &&env){
                // The envelope data structure provide further details, such as sampleTimePoint as shown in this test case:
                // https://github.com/chrberger/libcluon/blob/master/libcluon/testsuites/TestEnvelopeConverter.cpp#L31-L40
                std::lock_guard<std::mutex> lck(gsrMutex);
                gsr = cluon::extractMessage<opendlv::proxy::GroundSteeringRequest>(std::move(env));
                //std::cout << "lambda: groundSteering = " << gsr.groundSteering() << std::endl;
            };
 
            od4.dataTrigger(opendlv::proxy::GroundSteeringRequest::ID(), onGroundSteeringRequest);
           


            //declaration of Variables related Image segmentation and color detection

            cv::Mat leftImg;				//left segment with just cones 
            cv::Mat rightImg;				//right segment with just cones
            cv::Mat leftArray [5];			//smaller segments from leftImage
            cv::Mat rightArray [5];			//smaller segments from rightImage
            bool leftBooleans [5];			//boolean indication for cones presence
            bool rightBooleans [5];			//boolean indication for cones presence
            cv::Mat hsvImg;    				// HSV image for the original frame

            //std::vector v3fCircles;  // 3 element vector of floats, this will be the pass by reference output of HoughCircles()

            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning()) {
                // OpenCV data structure to hold an image.
                cv::Mat img;
                // Wait for a notification of a new frame.
                sharedMemory->wait();
 
                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }
                // TODO: Here, you can add some code to check the sampleTimePoint when the current frame was captured.
                sharedMemory->unlock();
 
                // TODO: Do something with the frame.

                //draw the red triangles for parts to discard in the main image
                cv::rectangle(img, cv::Point(2, 2), cv::Point(638, 248), cv::Scalar(0,0,255),4);
                cv::rectangle(img, cv::Point(2, 362), cv::Point(638, 478), cv::Scalar(0,0,255),4);

                //draw the green rectangles for the working areas
                cv::rectangle(img, cv::Point(2, 252), cv::Point(318, 358), cv::Scalar(0,255,0), 4);
                cv::rectangle(img, cv::Point(322, 252), cv::Point(638, 358), cv::Scalar(0,255,0), 4);


                //*********************Image Manipulation for Color detection ****************	
				// Apply gamma color correction by factor 0.4 
                double gamma_ = 0.4;
                cv::Mat lookUpTable(1, 256, CV_8U);
                uchar* p = lookUpTable.ptr();
                for( int i = 0; i < 256; ++i)
                    p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
                cv::Mat gamma_corrected = img.clone();
                cv::LUT(img, lookUpTable, gamma_corrected);

                //set HSV values
                int lowH = 105;
                int highH = 140;
                int lowS = 42;
                int highS = 107;
                int lowV = 107;
                int highV = 163;

                //set HSV values for yellow cones
                int lowH2 = 20;
                int highH2 = 30;
                int lowS2 = 0;
                int highS2 = 255;
                int lowV2 = 20;
                int highV2 = 255;

                cv::Mat blue_cones;   // Thresh Image
                cv::Mat yellow_cones; // Tresh Image

                // convert the gamma corrected image to HSV
                cv::cvtColor(gamma_corrected, hsvImg, CV_BGR2HSV);
                // Apply HSV filter
                cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), blue_cones);
                cv::inRange(hsvImg, cv::Scalar(lowH2, lowS2, lowV2), cv::Scalar(highH2, highS2, highV2), yellow_cones);


                // Erosion
                cv::Mat erosion_kernel =cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
                cv::erode(blue_cones, blue_cones, erosion_kernel);
                cv::erode(yellow_cones, yellow_cones, erosion_kernel);


                // Dilation
                cv::Mat dilation_kernel =cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
                cv::dilate(blue_cones, blue_cones, dilation_kernel);
                cv::dilate(yellow_cones, yellow_cones, dilation_kernel);

                
                
                cv::GaussianBlur(blue_cones, blue_cones, cv::Size(3, 3), 0);   //Blur Effect
                cv::GaussianBlur(yellow_cones, yellow_cones, cv::Size(3, 3), 0);   //Blur Effect
                
               	
               	/*addWeighted is commented here below cz we dont wanna blend both 
               	blue and yellow colors in one image
               	cv::Mat cones_image;
               	cv::addWeighted(blue_cones, 1.0, yellow_cones, 1.0, 0.0, cones_image);
               	cv::GaussianBlur(cones_image, cones_image, cv::Size(3, 3), 0); */


                /************************************
                *************VERY IMPORTANT**********
                *************Scenario Boolean********
                ************************************/
                int blueOnLeft = -1;				//the blue cone are Not on the left side -> default	
                /************************************
                ********THE METHOD FOR CHECKING******
                ********THE INITIAL CONES SETTING ***
                ********SHOULD BE CALLED HERE********
                ************************************/


               	//******************************************************************************
               	//-------------------------- Image segmentation and division in Arrays ---------

				/*//cropping the main part of the image from the Original Frame*/
                //cones_image(cv::Rect(0,250,640,100)).copyTo(workingArea);

                //cropping the left and right sides from the workingArea

                if (blueOnLeft==-1){
	                yellow_cones(cv::Rect(0,250,320,110)).copyTo(leftImg);
	                blue_cones(cv::Rect(320,250,320,110)).copyTo(rightImg);
            	} else {
            		yellow_cones(cv::Rect(0,250,320,110)).copyTo(rightImg);
	                blue_cones(cv::Rect(320,250,320,110)).copyTo(leftImg);
            	}
                //creation of the left frame segments
                int maxWidth = 320;
                int counter = 0;
                for (int i = 5; i > 0; i--)
                {
                	leftImg(cv::Rect(maxWidth-64,0,64,110)).copyTo(leftArray[counter]);
                	maxWidth = maxWidth - 64;
                	counter++;
                }

                //creation of the Right frame segments
                int startPoint = 0;
				for (int i = 0; i < 5; i++)
                {
                	rightImg(cv::Rect(startPoint,0,64,110)).copyTo(rightArray[i]);
                	startPoint = startPoint + 64;
                }

                //Call to THE FUNCTION to check the presence of the cones in the segments

                for (int i = 0; i < 5; ++i)
                {
                	rightBooleans[i] = checkConePresence(rightArray[i]);
                	leftBooleans[i] = checkConePresence(leftArray[i]);
                }

                for (int i = 0; i < 5; ++i)
                {
                	std::cout << rightBooleans[i] <<"    "<< leftBooleans[i] << std::endl;
                }

                //------------------------------Finish Image segmentation-----------------------
               	//******************************************************************************





                /*cv::namedWindow("cones", CV_WINDOW_AUTOSIZE);
                cv::namedWindow("1", CV_WINDOW_AUTOSIZE);
                cv::namedWindow("2", CV_WINDOW_AUTOSIZE);
                cv::namedWindow("3", CV_WINDOW_AUTOSIZE);
                cv::namedWindow("4", CV_WINDOW_AUTOSIZE);
                cv::namedWindow("5", CV_WINDOW_AUTOSIZE);
				*/
                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    /*cv::imshow("cones", leftImg);
                    cv::imshow("1", rightArray[0]);
                    cv::imshow("2", rightArray[1]);
                    cv::imshow("3", rightArray[2]);
                    cv::imshow("4", rightArray[3]);
                    cv::imshow("5", rightArray[4]);*/
                    cv::waitKey(1);
                }
            }
        }
        retCode = 0;
    }
    return retCode;
}



