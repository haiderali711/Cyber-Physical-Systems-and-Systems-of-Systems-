#define CATCH_CONFIG_MAIN  

#include "catch.hpp"
#include "template-opencv.cpp"

TEST_CASE("Test if cones are present on one side of the video frame: ") {
	/**
	Upload Left and Right Images.(Unfinished) 
	for the whole left side 

	*/
	cv::Mat leftImg;
	cv::Mat rightImg;
	SECTION("Checking the presence of cones on the whole left segment"){
		REQUIRE( checkConePresence(leftImg) == true );
	}
	SECTION("Checking the presence of coens on the whole right segment"){
		REQUIRE( checkConePresence(rightImg) == false );
	}
	SECTION("Checking cone presence on smaller segment of the left side"){
		REQUIRE( checkConePresence(leftImg[0]) == false );
	}
}