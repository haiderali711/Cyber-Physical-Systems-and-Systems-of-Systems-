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
#include <sys/time.h>
#include "ConeDetection.hpp"
#include "SteeringCalculator.hpp"

/************************************
*************VERY IMPORTANT**********
*************Scenario Boolean********
************************************/
int BLUE_IS_LEFT = 1; //the blue cone are Not on the left side -> default
double STEERING_TO_APPLY;

//*****************MAIN**********************
 
int32_t main(int32_t argc, char **argv) {
    int32_t retCode{1};
    ConeDetection coneDetection;
    SteeringCalculator steeringCalculator;

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
		

		//needed for gettinng current time
	    struct timeval  tv;
	    //time in decimals of a second
	    gettimeofday(&tv, NULL);
	    double start_time;
	    
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
	        
	        BLUE_IS_LEFT = coneDetection.decideSideCones(img);

	        // Endless loop; end the program by pressing Ctrl-C.
	        while (od4.isRunning()) {
				
	        	start_time = (tv.tv_sec)*10 + (tv.tv_usec) / 100000;


	            // Wait for a notification of a new frame.
	            sharedMemory->wait();

				//calculate current time
	            gettimeofday(&tv, NULL);
                double current_time = (tv.tv_sec)*10 + (tv.tv_usec) / 100000;

                //Decide whether the cones on the left are blue or yellow
                if ((int)(current_time - start_time) % 10 == 0) {
                    BLUE_IS_LEFT = coneDetection.decideSideCones(img);

                    if (BLUE_IS_LEFT==1){
	                   	std::cout << std::endl <<"Left : BLUE  || right : YELLOW"<< std::endl;
                    }  
                    if (BLUE_IS_LEFT==-1){
                       	std::cout << std::endl <<"Left : YELLOW  || right : BLUE"<< std::endl;
                    }     
	            }


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


                //Calculate the Steeing Angle 

                STEERING_TO_APPLY = steeringCalculator.calculateSteering(img,BLUE_IS_LEFT);
                std::cout << std::endl << "STEERING: "<<STEERING_TO_APPLY;


                std::string overlay;

				{
                    std::lock_guard<std::mutex> lck(gsrMutex);
					overlay += "Original: ";
                	overlay += std::to_string(gsr.groundSteering());
                	overlay += "   ||   Our : ";
                	overlay += std::to_string(STEERING_TO_APPLY);
                }

                cv::putText(img, overlay, cv::Point(20, 20), 1, 1,  cv::Scalar(255,255,255));





	            // Display image on your screen.
	            if (VERBOSE) {
	                cv::imshow(sharedMemory->name().c_str(), img);

                    //cv::imshow("b_cones", blue_cones);
                    //cv::imshow("y_cones", yellow_cones);
                    
                    //cv::imshow("conesL", leftImg);
                    //cv::imshow("conesR", rightImg);

                    cv::waitKey(1);
                }


            }
        }
        retCode = 0;
    }
    return retCode;
}

