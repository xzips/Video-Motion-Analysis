#include <vector>
#include <string>
#include <iostream>
#include <cstdio>


const std::string ffmpegBasePath = "C:\\Users\\aspen\\Desktop\\ffmpeg\\bin\\";

//C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\earth_video.mp4


//Video to new vector of pixel arrays for each frame, note returned char *'s are new allocated and must be deleted
std::vector<unsigned char*> vid2pixels(std::string videopath, int startFrame = 0, int endFrame = -1)
{

    std::string command = ffmpegBasePath + "ffmpeg -i " + videopath + " -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -";

    // Increased buffer size
    std::vector<char> buffer(16384); // Adjust buffer size as needed

    FILE* pipe = _popen(command.c_str(), "rb"); // Open pipe in binary mode

    if (!pipe) {
        std::cerr << "Error in vid2pixels: _popen() failed!" << std::endl;
        return { nullptr };
    }

    size_t bytesRead;
    while ((bytesRead = fread(buffer.data(), 1, buffer.size(), pipe)) > 0)
    {
        // Process the frame data here. Directly use buffer.data() and bytesRead for processing.
    }

    if (ferror(pipe)) {
        std::cerr << "Error in vid2pixels: Reading from pipe failed!" << std::endl;
    }

    _pclose(pipe);

    
}


std::tuple<int, int, double> vid_info(std::string videopath)
{
    TODO

}