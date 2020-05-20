#include "ConeDetection.hpp"


//Threshhold to detect cones in image (white percentage related to black space in HSV image)
double HAS_CONES_THRESHHOLD = 0.2;
cv::Mat erosion_kernel =cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
cv::Mat dilation_kernel =cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

/********************************************
****************CONE CHECKING FUNCTION*******
********************************************/


bool ConeDetection::checkConePresence (cv::Mat croppedImage) {
    int amount = 0;

    //To loop through all the pixels
    for (int x = 0;x < croppedImage.rows; x++) {
        for (int y = 0; y < croppedImage.cols; y++) {
            if(croppedImage.at<uchar>(x,y) == 255) {
                amount++;
            }
        }
    }
    int total_pixels = (croppedImage.rows)*croppedImage.cols;
    double white_percentage = (double)(amount * 100) / total_pixels;

    if (white_percentage > HAS_CONES_THRESHHOLD) {
        return true;
    }
    return false;
}

cv::Mat ConeDetection::applyGammaCorrection (cv::Mat img){
    // Apply gamma color correction by factor 0.4
    double gamma_ = 0.4;
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for( int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);
    cv::Mat gamma_corrected = img.clone();
    cv::LUT(img, lookUpTable, gamma_corrected);

    return gamma_corrected;
}



cv::Mat ConeDetection::applyBlueFilter (cv::Mat img) {
    //set HSV values for blue cones
    int lowH = 108;
    int highH = 128;
    int lowS = 108;
    int highS = 255;
    int lowV = 10;
    int highV = 255;

    cv::Mat blue_cones;   // Thresh Image
    cv::Mat hsvImg;

    cv::Mat gamma_corrected = img;

    // convert the gamma corrected image to HSV
    cv::cvtColor(gamma_corrected, hsvImg, CV_BGR2HSV);

    // Apply HSV filter
    cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), blue_cones);

    // Erosion
    cv::erode(blue_cones, blue_cones, erosion_kernel);

    // Dilation
    cv::dilate(blue_cones, blue_cones, dilation_kernel);

    return blue_cones;
}

cv::Mat ConeDetection::applyYellowFilter (cv::Mat img) {

    //set HSV values for yellow cones
    int lowH2 = 20;
    int highH2 = 35;
    int lowS2 = 100;
    int highS2 = 230;
    int lowV2 = 115;
    int highV2 = 255;

    cv::Mat yellow_cones; // Tresh Image
    cv::Mat hsvImg;

    // convert the gamma corrected image to HSV
    cv::cvtColor(img, hsvImg, CV_BGR2HSV);

    // Apply HSV filter
    cv::inRange(hsvImg, cv::Scalar(lowH2, lowS2, lowV2), cv::Scalar(highH2, highS2, highV2), yellow_cones);

    // Erosion
    cv::erode(yellow_cones, yellow_cones, erosion_kernel);

    // Dilation
    cv::dilate(yellow_cones, yellow_cones, dilation_kernel);

    return yellow_cones;
}

/********************************************
****************SIDE CHECKING FUNCTION*******
********************************************/
int ConeDetection::decideSideCones (cv::Mat img, int BLUE_IS_LEFT) {
    cv::Mat left,right;

    //apply filters the other way around
    if (BLUE_IS_LEFT == 1) {
        right = applyBlueFilter(img);
        left = applyYellowFilter(img);
    } else {
        left = applyBlueFilter(img);
        right = applyYellowFilter(img);
    }
    //crop image
    left(cv::Rect(0,250,320,110)).copyTo(left);
    right(cv::Rect(320,250,320,110)).copyTo(right);

    //if the filters applied the other way detect cones then it means that
    //the variables indicating in which side the blue cones are has to be switched
    if (checkConePresence(left) && checkConePresence(right)) {
        BLUE_IS_LEFT = BLUE_IS_LEFT * -1;
    }

}

int ConeDetection::decideCones (cv::Mat img) {
    cv::Mat left,right;

    right = applyBlueFilter(img);
    left = applyBlueFilter(img);

    //crop image
    left(cv::Rect(0,250,320,110)).copyTo(left);
    right(cv::Rect(320,250,320,110)).copyTo(right);

    if (checkConePresence(left)){
        return 1;
    }else {
        return -1;
    }
}
