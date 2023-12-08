#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "vid2pixels.hpp"
#include <tuple>
#include "SFML/Graphics.hpp"
#include "DirectDifference.hpp"
#include "TranslatedDifference.hpp"


void PlayVideoDemo() {

    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\countdown_video.mp4\"";


    std::tuple<int, int, double, int> video_info = vid_info(vidpath);

    //std cout the video info
    std::cout << "Video info: " << std::endl;
    std::cout << "Width: " << std::get<0>(video_info) << std::endl;
    std::cout << "Height: " << std::get<1>(video_info) << std::endl;
    std::cout << "Framerate: " << std::get<2>(video_info) << std::endl;
    std::cout << "Number of frames: " << std::get<3>(video_info) << std::endl;

    std::vector<unsigned char*> frames = vid2pixels(vidpath);
    


    // Ensure there's at least one frame in the vector
    if (!frames.empty()) {
        int frame_index = 0;

        int width, height;
        std::tie(width, height, std::ignore, std::ignore) = video_info;

        sf::Texture texture;
        if (!texture.create(width, height)) {
            std::cerr << "Failed to create texture" << std::endl;
        }

        // Update the texture with the pixel data from the first frame
        texture.update(frames[frame_index]);

        sf::Sprite sprite;
        sprite.setTexture(texture);

        sf::RenderWindow window(sf::VideoMode(width, height), "Video Playback Demo");
        //set framerate to video framerate
        window.setFramerateLimit((int)std::get<2>(video_info));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(sprite);
            // update texture if we are less than total length

            if (frame_index < std::get<3>(video_info) - 1) {
                frame_index += 1;
                texture.update(frames[frame_index]);
            }
            else {
                frame_index = 0;
            }



            window.display();
        }

        // Cleanup: Remember to delete the allocated frame data to avoid memory leaks
        for (auto& frame : frames) {
            delete[] frame;
        }
    }

}


void DirectDifferenceDemo() {
    //std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\earth_video.mp4\"";
    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\drop_video.mp4\"";

    std::tuple<int, int, double, int> video_info = vid_info(vidpath);

    // std::cout the video info
    std::cout << "Video info: " << std::endl;
    std::cout << "Width: " << std::get<0>(video_info) << std::endl;
    std::cout << "Height: " << std::get<1>(video_info) << std::endl;
    std::cout << "Framerate: " << std::get<2>(video_info) << std::endl;
    std::cout << "Number of frames: " << std::get<3>(video_info) << std::endl;

    int actual_length_frames = 150;

    std::vector<unsigned char*> frames = vid2pixels(vidpath, 0, actual_length_frames);

    if (frames.size() > 1) {
        int frame_index = 0;
        int next_frame_index = 1;

        int width, height;
        std::tie(width, height, std::ignore, std::ignore) = video_info;

        // Create two textures: one for the frame difference, one for the current frame
        sf::Texture diffTexture, currentFrameTexture;
        if (!diffTexture.create(width, height) || !currentFrameTexture.create(width, height)) {
            std::cerr << "Failed to create textures" << std::endl;
        }

        // Create two sprites
        sf::Sprite diffSprite, currentFrameSprite;
        diffSprite.setTexture(diffTexture);
        currentFrameSprite.setTexture(currentFrameTexture);

        // Position the sprites
        currentFrameSprite.setPosition(0, height);

        // Create a window twice the height of the frames
        sf::RenderWindow window(sf::VideoMode(width, height * 2), "Direct Difference Demo");
        window.setFramerateLimit((int)std::get<2>(video_info));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            // Compute and display the difference between frames
            unsigned char* diff = DirectDifference(frames[frame_index], frames[next_frame_index], width, height);
            diffTexture.update(diff);
            delete[] diff; // Clean up the diff frame

            // Display the current frame
            currentFrameTexture.update(frames[frame_index]);

            window.draw(diffSprite);
            window.draw(currentFrameSprite);

            // Increment and loop frame indices
            frame_index = next_frame_index;
            next_frame_index++;
            if (next_frame_index >= actual_length_frames) {
                next_frame_index = 0;
            }

            window.display();
        }

        // Cleanup: Delete the original frames
        for (auto& frame : frames) {
            delete[] frame;
        }
    }
}



void TranslatedDifferenceDemo() {
    //std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\earth_video.mp4\"";
    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\pendulum_video.mp4\"";

    std::tuple<int, int, double, int> video_info = vid_info(vidpath);

    // std::cout the video info
    std::cout << "Video info: " << std::endl;
    std::cout << "Width: " << std::get<0>(video_info) << std::endl;
    std::cout << "Height: " << std::get<1>(video_info) << std::endl;
    std::cout << "Framerate: " << std::get<2>(video_info) << std::endl;
    std::cout << "Number of frames: " << std::get<3>(video_info) << std::endl;

    int actual_length_frames = 150;

    std::vector<unsigned char*> frames = vid2pixels(vidpath, 0, actual_length_frames);

    if (frames.size() > 1) {
        int frame_index = 0;
        int next_frame_index = 1;

        int width, height;
        std::tie(width, height, std::ignore, std::ignore) = video_info;

        // Create two textures: one for the frame difference, one for the current frame
        sf::Texture diffTexture, currentFrameTexture;
        if (!diffTexture.create(width, height) || !currentFrameTexture.create(width, height)) {
            std::cerr << "Failed to create textures" << std::endl;
        }

        // Create two sprites
        sf::Sprite diffSprite, currentFrameSprite;
        diffSprite.setTexture(diffTexture);
        currentFrameSprite.setTexture(currentFrameTexture);

        // Position the sprites
        currentFrameSprite.setPosition(0, height);

        // Create a window twice the height of the frames
        sf::RenderWindow window(sf::VideoMode(width, height * 2), "Direct Difference Demo");
        window.setFramerateLimit((int)std::get<2>(video_info));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            // Compute and display the difference between frames
            unsigned char* diff = TranslatedDifference(frames[frame_index], frames[next_frame_index], width, height, 3);
            diffTexture.update(diff);
            delete[] diff; // Clean up the diff frame

            // Display the current frame
            currentFrameTexture.update(frames[frame_index]);

            window.draw(diffSprite);
            window.draw(currentFrameSprite);

            // Increment and loop frame indices
            frame_index = next_frame_index;
            next_frame_index++;
            if (next_frame_index >= actual_length_frames) {
                next_frame_index = 0;
            }

            window.display();
        }

        // Cleanup: Delete the original frames
        for (auto& frame : frames) {
            delete[] frame;
        }
    }
}
