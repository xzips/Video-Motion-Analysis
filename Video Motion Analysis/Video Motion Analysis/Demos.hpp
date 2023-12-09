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
#include "OpticalFlow.hpp"

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
            unsigned char* diff = TranslatedDifference(frames[frame_index], frames[next_frame_index], width, height, 2);
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


void OpticalFlowDemo() {
    //std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\pendulum_video.mp4\"";
    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\components_video.mp4\"";
    std::tuple<int, int, double, int> video_info = vid_info(vidpath);

    // Print video info
    std::cout << "Video info: " << std::endl;
    std::cout << "Width: " << std::get<0>(video_info) << std::endl;
    std::cout << "Height: " << std::get<1>(video_info) << std::endl;
    std::cout << "Framerate: " << std::get<2>(video_info) << std::endl;
    std::cout << "Number of frames: " << std::get<3>(video_info) << std::endl;

    int actual_length_frames = 1000;

	//if actual_length_frames is more than the number of frames in the video, set it to the number of frames in the video
	if (actual_length_frames > std::get<3>(video_info)) {
		actual_length_frames = std::get<3>(video_info);
	}
    


    std::vector<unsigned char*> frames = vid2pixels(vidpath, 0, actual_length_frames);

    if (frames.size() > 1) {
        int frame_index = 0;
        int next_frame_index = 1;

        int width, height;
        std::tie(width, height, std::ignore, std::ignore) = video_info;

        sf::Texture frameTexture;
        if (!frameTexture.create(width, height)) {
            std::cerr << "Failed to create texture" << std::endl;
        }

        sf::Sprite frameSprite;
        frameSprite.setTexture(frameTexture);

        sf::RenderWindow window(sf::VideoMode(width, height), "Optical Flow Demo");
        window.setFramerateLimit((int)std::get<2>(video_info));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();

            // Compute optical flow
            vector2d flow = OpticalFlow(frames[frame_index], frames[next_frame_index], width, height);
            //print flow
			std::cout << "Flow: (" << flow.x << ", " << flow.y << ")" << std::endl;

            flow.x = -flow.x;
			flow.y = -flow.y;

            // Update and draw the frame
            frameTexture.update(frames[frame_index]);
            window.draw(frameSprite);

            // Draw the arrow representing the flow
            sf::RectangleShape arrowShaft(sf::Vector2f(std::sqrt(flow.x * flow.x + flow.y * flow.y) * 10, 2));
            arrowShaft.setPosition(width / 2, height / 2);
            arrowShaft.setRotation(std::atan2(flow.y, flow.x) * 180 / 3.14159);
			arrowShaft.setFillColor(sf::Color::Red);
            

            window.draw(arrowShaft);

			//draw arrow head as a triangle
            // Calculate the angle of the arrow
            float angle = std::atan2(flow.y, flow.x) * 180 / 3.14159;

            // Create an arrowhead as a triangle
            sf::ConvexShape arrowHead;
            arrowHead.setPointCount(3);

            // Size and shape of the arrowhead
            float arrowHeadLength = 10.0f; // You can adjust this value
            float arrowHeadWidth = 6.0f; // You can adjust this value

            // Set the points of the triangle
            arrowHead.setPoint(0, sf::Vector2f(0, -arrowHeadWidth));
            arrowHead.setPoint(1, sf::Vector2f(arrowHeadLength, 0));
            arrowHead.setPoint(2, sf::Vector2f(0, arrowHeadWidth));

            // Position and rotate the arrowhead
            // Adjust the position so that the arrowhead is at the end of the shaft
            float shaftLength = std::sqrt(flow.x * flow.x + flow.y * flow.y) * 10;
            arrowHead.setPosition(width / 2 + shaftLength * std::cos(angle * 3.14159 / 180),
                height / 2 + shaftLength * std::sin(angle * 3.14159 / 180));
            arrowHead.setRotation(angle);

            // Set the color of the arrowhead
            arrowHead.setFillColor(sf::Color::Red);

            // Draw the arrowhead
            window.draw(arrowHead);
            


            // Increment and loop frame indices
            frame_index = next_frame_index;
            next_frame_index++;
            if (next_frame_index + 1 >= actual_length_frames) {
                frame_index = 0;
                next_frame_index = 1;
            }

            window.display();
        }

        // Cleanup
        for (auto& frame : frames) {
            delete[] frame;
        }
    }
}