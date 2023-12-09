#include "OpticalFlow.hpp"
#include <math.h>
#include <cmath>
#include <limits>



//works ok but is slow asf
/*
vector2d OpticalFlow(unsigned char* frame1, unsigned char* frame2, int width, int height) {
    int block_size = 20; //10
    int search_window = 20; //20

    int half_block = block_size / 2;
    int half_search = search_window / 2;

    float avg_dx = 0.0f, avg_dy = 0.0f;
    int count = 0;

    for (int y = half_block; y < height - half_block; y += block_size) {
        for (int x = half_block; x < width - half_block; x += block_size) {
            int best_dx = 0, best_dy = 0;
            float min_diff = std::numeric_limits<float>::max();

            for (int dy = -half_search; dy <= half_search; ++dy) {
                for (int dx = -half_search; dx <= half_search; ++dx) {
                    if (y + dy < half_block || y + dy >= height - half_block ||
                        x + dx < half_block || x + dx >= width - half_block) {
                        continue; // Skip if search window goes out of bounds
                    }

                    float diff = 0.0f;

                    for (int by = -half_block; by <= half_block; ++by) {
                        for (int bx = -half_block; bx <= half_block; ++bx) {
                            int idx1 = ((y + by) * width + (x + bx)) * 3; // Assuming 3 channels (RGB)
                            int idx2 = ((y + dy + by) * width + (x + dx + bx)) * 3;

                            for (int i = 0; i < 3; ++i) { // For each color channel
                                diff += std::abs(static_cast<int>(frame1[idx1 + i]) - static_cast<int>(frame2[idx2 + i]));
                            }
                        }
                    }

                    if (diff < min_diff) {
                        min_diff = diff;
                        best_dx = dx;
                        best_dy = dy;
                    }
                }
            }

            avg_dx += best_dx;
            avg_dy += best_dy;
            count++;
        }
    }

    vector2d avg_motion;
    if (count > 0) {
        avg_motion.x = static_cast<int>(std::round(avg_dx / count));
        avg_motion.y = static_cast<int>(std::round(avg_dy / count));
    }
    else {
        avg_motion.x = 0;
        avg_motion.y = 0;
    }

    return avg_motion;
}*/


vector2d OpticalFlow(unsigned char* frame1, unsigned char* frame2, int width, int height) {
    int blocks_x = 10;
	int blocks_y = 10;


    int search_window = 20; //20
    int block_size = 20; // You can still set a default or calculate based on image size and blocks_x, blocks_y

    int half_block = block_size / 2;
    int half_search = search_window / 2;

    // Calculate the spacing between the blocks
    int x_step = width / blocks_x;
    int y_step = height / blocks_y;

    float avg_dx = 0.0f, avg_dy = 0.0f;
    int count = 0;

    for (int y = half_block; y < height - half_block; y += y_step) {
        for (int x = half_block; x < width - half_block; x += x_step) {
            int best_dx = 0, best_dy = 0;
            float min_diff = std::numeric_limits<float>::max();

            for (int dy = -half_search; dy <= half_search; ++dy) {
                for (int dx = -half_search; dx <= half_search; ++dx) {
                    if (y + dy < half_block || y + dy >= height - half_block ||
                        x + dx < half_block || x + dx >= width - half_block) {
                        continue; // Skip if search window goes out of bounds
                    }

                    float diff = 0.0f;

                    for (int by = -half_block; by <= half_block; ++by) {
                        for (int bx = -half_block; bx <= half_block; ++bx) {
                            int idx1 = ((y + by) * width + (x + bx)) * 3; // Assuming 3 channels (RGB)
                            int idx2 = ((y + dy + by) * width + (x + dx + bx)) * 3;

                            for (int i = 0; i < 3; ++i) { // For each color channel
                                diff += std::abs(static_cast<int>(frame1[idx1 + i]) - static_cast<int>(frame2[idx2 + i]));
                            }
                        }
                    }

                    if (diff < min_diff) {
                        min_diff = diff;
                        best_dx = dx;
                        best_dy = dy;
                    }
                }
            }

            avg_dx += best_dx;
            avg_dy += best_dy;
            count++;
        }
    }

    vector2d avg_motion;
    if (count > 0) {
        avg_motion.x = static_cast<int>(std::round(avg_dx / count));
        avg_motion.y = static_cast<int>(std::round(avg_dy / count));
    }
    else {
        avg_motion.x = 0;
        avg_motion.y = 0;
    }

    return avg_motion;
}