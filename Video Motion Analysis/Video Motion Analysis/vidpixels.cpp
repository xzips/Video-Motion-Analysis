#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <regex>
#include <tuple>
#include "vid2pixels.hpp"


const std::string ffmpegBasePath = "C:\\Users\\aspen\\Desktop\\ffmpeg\\bin\\";

//C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\earth_video.mp4


//Video to new vector of pixel arrays for each frame, note returned char *'s are new allocated and must be deleted
std::vector<unsigned char*> vid2pixels(std::string videopath, int startFrame, int endFrame)
{
    // Call vid_info and then unpack the tuple
    auto info = vid_info(videopath);

    int width, height, frame_count;
    double framerate;
    std::tie(width, height, framerate, frame_count) = info;

    // Adjust endFrame if necessary
    if (endFrame == -1 || endFrame > frame_count) {
        endFrame = frame_count;
    }

    std::string command = ffmpegBasePath + "ffmpeg -i " + videopath + " -f image2pipe -vcodec rawvideo -pix_fmt rgba -";

    std::vector<unsigned char*> frames;

    FILE* pipe = _popen(command.c_str(), "rb");
    if (!pipe) {
        std::cerr << "Error in vid2pixels: _popen() failed!" << std::endl;
        return frames;
    }

    // Calculate the size of each frame (in bytes)
    size_t frameSize = width * height * 4; // 4 bytes per pixel for RGBA

    std::vector<char> buffer(frameSize);
    int currentFrame = 0;

    while (fread(buffer.data(), 1, buffer.size(), pipe) == frameSize)
    {
        if (currentFrame >= startFrame && currentFrame < endFrame) {
            unsigned char* frameData = new unsigned char[frameSize];
            std::memcpy(frameData, buffer.data(), frameSize);
            frames.push_back(frameData);
        }
        currentFrame++;

        // Break if end frame is reached
        if (currentFrame >= endFrame) {
            break;
        }
    }

    if (ferror(pipe)) {
        std::cerr << "Error in vid2pixels: Reading from pipe failed!" << std::endl;
    }

    _pclose(pipe);

    return frames;
}





std::tuple<int, int, double, int> vid_info(std::string videopath)
{
    std::string command = ffmpegBasePath + "ffprobe -v error -select_streams v:0 -show_entries stream=width,height,r_frame_rate,nb_frames -of default=noprint_wrappers=1 " + videopath;

    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: _popen() failed!" << std::endl;
        return std::make_tuple(-1, -1, -1.0, -1);
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }
    _pclose(pipe);

    // Regular expressions to parse the output
    std::regex width_regex("width=([0-9]+)");
    std::regex height_regex("height=([0-9]+)");
    std::regex framerate_regex("r_frame_rate=([0-9]+)/([0-9]+)");
    std::regex framecount_regex("nb_frames=([0-9]+)");

    std::smatch matches;
    int width = -1, height = -1, framecount = -1;
    double framerate = -1.0;

    if (std::regex_search(result, matches, width_regex) && matches.size() > 1) {
        width = std::stoi(matches.str(1));
    }
    if (std::regex_search(result, matches, height_regex) && matches.size() > 1) {
        height = std::stoi(matches.str(1));
    }
    if (std::regex_search(result, matches, framerate_regex) && matches.size() > 2) {
        framerate = std::stod(matches.str(1)) / std::stod(matches.str(2));
    }
    if (std::regex_search(result, matches, framecount_regex) && matches.size() > 1) {
        framecount = std::stoi(matches.str(1));
    }

    return std::make_tuple(width, height, framerate, framecount);
}
