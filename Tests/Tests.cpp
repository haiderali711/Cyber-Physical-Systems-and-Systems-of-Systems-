#define CATCH_CONFIG_MAIN  
#include "catch.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "ConeDetection.hpp"
#include "SteeringCalculator.hpp"

//using namespace cv;

ConeDetection cd;
SteeringCalculator st;
cv::Mat image;


TEST_CASE("Blue on left","[test1]") {
	image = cv::imread("../MD_Material/img3.jpg", -1);	
	REQUIRE(cd.decideSideCones(image) == 1);

	image = cv::imread("../MD_Material/img3-M.jpg", -1);	
	REQUIRE(cd.decideSideCones(image) == -1);
}
TEST_CASE("Cone presence","[test2]") {
	image = cv::imread("../MD_Material/noCones.jpg", -1);	
	REQUIRE(cd.checkConePresence(image) == false);

	image = cv::imread("../MD_Material/withCones.jpg", -1);	
	REQUIRE(cd.checkConePresence(image) == true);
}

TEST_CASE("SteeringCalculator checker","[test3]") {
	image = cv::imread("../MD_Material/img3.jpg", -1);	
	REQUIRE(st.calculateSteering(image,1) <= 0);
}

/*TEST_CASE("SteeringCalculator checker", "[test3]") {
	image = cv::imread("../MD_Material/img3-M.jpg", -1);	
	CHECK(st.calculateSteering(image,-1) >= 0);
}*/