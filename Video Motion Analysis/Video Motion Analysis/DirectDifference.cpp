

#include <math.h>



unsigned char* DirectDifference(unsigned char* frame1, unsigned char* frame2, int width, int height) {
	// Create a new array to store the difference between the two frames
	unsigned char* diff = new unsigned char[width * height * 4];

	// Loop through each pixel in the frame
	for (int i = 0; i < width * height * 4; i += 4) {
		// Calculate the difference between the two frames, absolute value
		diff[i] = abs(frame1[i] - frame2[i]);
		diff[i + 1] = abs(frame1[i + 1] - frame2[i + 1]);
		diff[i + 2] = abs(frame1[i + 2] - frame2[i + 2]);

		//average the differences, then set that as the pixel value greyscale
		diff[i] = (diff[i] + diff[i + 1] + diff[i + 2]) / 3;
		diff[i + 1] = diff[i];
		diff[i + 2] = diff[i];

		diff[i + 3] = 255; // Alpha channel

	}

	return diff;
}

