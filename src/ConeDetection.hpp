#ifndef CONEDETECTION
#define CONEDETECTION
#include <cstdint>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class ConeDetection {
   public:
    bool checkConePresence (cv::Mat image);
    cv::Mat applyGammaCorrection (cv::Mat img);
	cv::Mat applyBlueFilter (cv::Mat img);
	cv::Mat applyYellowFilter (cv::Mat img);
    int decideSideCones (cv::Mat img);
};
#endif
