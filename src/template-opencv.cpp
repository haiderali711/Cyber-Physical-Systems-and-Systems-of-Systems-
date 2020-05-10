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
            //time_t now;
            //int seconds, microseconds;
           
            cv::Mat workingArea;
            cv::Mat leftImg;
            cv::Mat rightImg;

            //----------------Object detection
            int lowHB = 108;       // Set Hue
            int highHB = 128;

            int lowSB = 90;       // Set Saturation
            int highSB = 255;

            int lowVB = 10;       // Set Value
            int highVB = 225; 
            /*
            int lowHY = 20;       // Set Hue
            int highY = 30;

            int lowSY = 0;       // Set Saturation
            int highSY = 255;

            int lowVY = 20;       // Set Value
            int highVY = 225; 
            */
            cv::Mat hsvImg;    // HSV Image
            cv::Mat threshImg;   // Thresh Image

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
                    //seconds = std::get<cluon::data::TimeStamp>(sharedMemory->getTimeStamp()).seconds();
                    //microseconds = std::get<cluon::data::TimeStamp>(sharedMemory->getTimeStamp()).microseconds();
                }
                // TODO: Here, you can add some code to check the sampleTimePoint when the current frame was captured.
                sharedMemory->unlock();
 
                // TODO: Do something with the frame.

                //cropping the main part of the image from the Original Frame
                img(cv::Rect(0,250,640,100)).copyTo(workingArea);
                //draw the red triangles for parts to discard in the main image
                cv::rectangle(img, cv::Point(2, 2), cv::Point(638, 248), cv::Scalar(0,0,255),4);
                cv::rectangle(img, cv::Point(2, 362), cv::Point(638, 478), cv::Scalar(0,0,255),4);

                //draw the green rectangles for the working areas
                cv::rectangle(img, cv::Point(2, 252), cv::Point(318, 358), cv::Scalar(0,255,0), 4);
                cv::rectangle(img, cv::Point(322, 252), cv::Point(638, 358), cv::Scalar(0,255,0), 4);

                //cropping the left and right sides from the workingArea
                img(cv::Rect(0,250,320,110)).copyTo(leftImg);
                img(cv::Rect(320,250,320,110)).copyTo(rightImg);


                //-------------------------------------------Object detection



                cv::cvtColor(rightImg, hsvImg, CV_BGR2HSV);      // Convert Original Image to HSV Thresh Image

                cv::inRange(hsvImg, cv::Scalar(lowHB, lowSB, lowVB), cv::Scalar(highHB, highSB, highVB), threshImg);

                cv::GaussianBlur(threshImg, threshImg, cv::Size(3, 3), 0);   //Blur Effect
                cv::dilate(threshImg, threshImg, 0);        // Dilate Filter Effect
                cv::erode(threshImg, threshImg, 0);         // Erode Filter Effect
               
                // Display image on your screen.
                if (VERBOSE) {
                    cv::imshow(sharedMemory->name().c_str(), rightImg);
                    cv::waitKey(1);
                }
            }
        }
        retCode = 0;
    }
    return retCode;
}
