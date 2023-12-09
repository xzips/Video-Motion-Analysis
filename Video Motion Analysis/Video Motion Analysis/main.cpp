#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "vid2pixels.hpp"
#include <tuple>
#include "Demos.hpp"
#include "SFML/Graphics.hpp"

int main() {


    //std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\countdown_video.mp4\"";
    //std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\pendulum_video.mp4\"";
    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\components_video.mp4\"";



    //PlayVideoDemo(vidpath);
    
    //DirectDifferenceDemo(vidpath);

	//TranslatedDifferenceDemo(vidpath);

    OpticalFlowDemo(vidpath);

    return 0;
}
