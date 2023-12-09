#pragma once


struct vector2d {
	double x;
	double y;
};

vector2d OpticalFlow(unsigned char* frame1, unsigned char* frame2, int width, int height);