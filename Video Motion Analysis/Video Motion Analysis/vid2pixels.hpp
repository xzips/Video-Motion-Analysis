/*
vid2pixels.hpp 
Aspen Erlandsson Brisebois 2023



/SUMMARY/
This file contains the function declarations for the vid2pixel.cpp file, and acomplishes one specific goal:
- Use FFMPEG to convert a video file into a series of images as raw pixel data chunks, returning a vector of pointers to each image's pixel data

Return type is unsigned char *, and follows the pixel format [R,G,B,A], left to right, top to bottom




/EXAMPLE/

For example, a 2x2 image with the pixels
image  =   [255,0,0, 255], [0,255,0, 255],
		   [0,0,255, 255], [255,255,255,255]

		   would be returned as:
[255,0,0,0,255,255,0,0,0,255,255,255,255,255,255] = [255,0,0,255,    0,255,0,255,    0,0,255,255,    255,255,255255,] (spaced added for clarity, but its just a 1D array of unsigned chars)


*/

#pragma once

#include <vector>
#include <string>
#include <tuple>

//Video to new vector of pixel arrays for each frame, note returned char *'s are new allocated and must be deleted
std::vector<unsigned char*> vid2pixels(std::string videopath, int startFrame = 0, int endFrame = -1);


//Returns relevant info about video, in the order: { width, height, framerate, frame_count }
std::tuple<int, int, double, int> vid_info(std::string videopath);