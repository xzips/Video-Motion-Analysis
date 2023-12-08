#include <math.h>
#include <climits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>









//plan is to first shift by bulk motion, then do discrete local difference
unsigned char* TranslatedDifference(unsigned char* frame1, unsigned char* frame2, int width, int height, int shift) {
    // Create a new array to store the difference between the two frames
    unsigned char* diff = new unsigned char[width * height * 4];

    // Loop through each pixel in the frame
    for (int i = 0; i < width * height * 4; i += 4) {
        // Initialize the minimum difference to the maximum possible value
        int min_diff = INT_MAX;

        // Loop through each possible shift in x and y
        for (int x = -shift; x <= shift; x++) {
            for (int y = -shift; y <= shift; y++) {
                // Calculate the difference between the two frames, absolute value
                int diff_x = i % (width * 4) + x * 4;
                int diff_y = i / (width * 4) + y;
                if (diff_x < 0 || diff_x >= width * 4 || diff_y < 0 || diff_y >= height) {
                    continue;
                }
                int diff_r = abs(frame1[i] - frame2[diff_y * width * 4 + diff_x]);
                int diff_g = abs(frame1[i + 1] - frame2[diff_y * width * 4 + diff_x + 1]);
                int diff_b = abs(frame1[i + 2] - frame2[diff_y * width * 4 + diff_x + 2]);

                // Calculate the average difference
                int avg_diff = (diff_r + diff_g + diff_b) / 3;

                // Update the minimum difference if necessary
                if (avg_diff < min_diff) {
                    min_diff = avg_diff;
                }
            }
        }

        // Set the pixel value to the minimum difference
        diff[i] = min_diff;
        diff[i + 1] = min_diff;
        diff[i + 2] = min_diff;
        diff[i + 3] = 255; // Alpha channel
    }

    return diff;
}
