#ifndef STEERINGCALCULATOR
#define STEERINGCALCULATOR
#include <cstdint>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class SteeringCalculator {
   public:
    double calculateSteering (cv::Mat image, int BLUE_IS_LEFT);
};
#endif
