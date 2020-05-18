#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "ConeDetection.hpp"

//using namespace cv;

TEST_CASE("test") {
	cv::Mat image = cv::imread("img.jpg", 0);
	ConeDetection cd;
	REQUIRE(cd.decideSideCones (image, -1)== -1);
}