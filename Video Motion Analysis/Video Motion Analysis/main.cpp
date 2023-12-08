#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "vid2pixels.hpp"


int main() {
    
    std::string vidpath = "C:\\Users\\aspen\\Desktop\\Video - Motion - Analysis\\earth_video.mp4";

    std::vector<unsigned char*> frames = vid2pixels(vidpath);

    std::tuple<int, int, double> video_info = vid_info(vidpath);



    return 0;
}
