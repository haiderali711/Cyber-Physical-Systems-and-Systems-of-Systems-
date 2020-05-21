#include "SteeringCalculator.hpp"
#include "ConeDetection.hpp"
#include <iostream>


//declaration of Variables related Image segmentation and color detection
int SEGMENTS = 10;
double STEERING_LIMIT = 0.290888;
double STEERING_ANGLE= 0.00;
double CONSTANT = STEERING_LIMIT / sin(1);



double SteeringCalculator::calculateSteering (cv::Mat img, int BLUE_IS_LEFT) {
    ConeDetection coneDetection;
    cv::Mat leftImg;                //left segment with just cones 
    cv::Mat rightImg;               //right segment with just cones
    cv::Mat leftArray [SEGMENTS];           //smaller segments from leftImage
    cv::Mat rightArray [SEGMENTS];          //smaller segments from rightImage
    bool leftBooleans [SEGMENTS];           //boolean indication for cones presence
    bool rightBooleans [SEGMENTS];          //boolean indication for cones presence
    cv::Mat hsvImg;                 // HSV image for the original frame

    //*********************Image Manipulation for Color detection ****************  
    cv::Mat blue_cones = coneDetection.applyBlueFilter(img);
    cv::Mat yellow_cones = coneDetection.applyYellowFilter(img);

    //******************************************************************************
    //-------------------------- Image segmentation and division in Arrays ---------

    //cropping the left and right sides from the workingArea

    if (BLUE_IS_LEFT == 1) {
        blue_cones(cv::Rect(0,250,320,110)).copyTo(leftImg);
        yellow_cones(cv::Rect(320,250,320,110)).copyTo(rightImg);
    } else {
        yellow_cones(cv::Rect(0,250,320,110)).copyTo(leftImg);
        blue_cones(cv::Rect(320,250,320,110)).copyTo(rightImg);
    }


    //creation of the left frame segments
    int maxWidth = 320;
    int slotWidth = maxWidth / SEGMENTS;
    int counter = 0;

    for (int i = SEGMENTS; i > 0; i--)
    {
        leftImg(cv::Rect(maxWidth-slotWidth,0,slotWidth,110)).copyTo(leftArray[counter]);
        maxWidth = maxWidth - slotWidth;
        counter++;
    }

    //creation of the Right frame segments
    int startPoint = 0;
    for (int i = 0; i < SEGMENTS; i++)
    {
        rightImg(cv::Rect(startPoint,0,slotWidth,110)).copyTo(rightArray[i]);
        startPoint = startPoint + slotWidth;
    }

    //Call to THE FUNCTION to check the presence of the cones in the segments

    int countLeft = 0,countRight = 0;
                    
    for (int i = 0; i < SEGMENTS; ++i)
    {
        rightBooleans[i] = coneDetection.checkConePresence(rightArray[i]);
        leftBooleans[i] = coneDetection.checkConePresence(leftArray[i]);
        if (rightBooleans[i] && countRight==0)  countRight = SEGMENTS - i;
        if (leftBooleans[i] && countLeft==0)  countLeft = SEGMENTS - i;
    }
    std::cout << std::endl << countLeft << " " << countRight;
	if(countLeft==0 && countRight==0){}
        else if (abs(countLeft-countRight)<=3) {STEERING_ANGLE=0;}
	       else{
    int delta = countRight - countLeft;
    //STEERING_ANGLE = delta*STEERING_UNIT;
    STEERING_ANGLE = (double)sin((double)(delta*delta*delta*delta*delta)/(SEGMENTS*SEGMENTS*SEGMENTS*SEGMENTS*SEGMENTS))*CONSTANT; 
	}

    return STEERING_ANGLE;
}

//------------------------------Finish Image segmentation-----------------------
//******************************************************************************
