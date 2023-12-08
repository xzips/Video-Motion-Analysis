#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include "vid2pixels.hpp"
#include <tuple>

#include "SFML/Graphics.hpp"

int main() {
    
    std::string vidpath = "\"C:\\Users\\aspen\\Desktop\\Video-Motion-Analysis\\countdown_video.mp4\"";


    std::tuple<int, int, double, int> video_info = vid_info(vidpath);

	//std cout the video info
	std::cout << "Video info: " << std::endl;
	std::cout << "Width: " << std::get<0>(video_info) << std::endl;
	std::cout << "Height: " << std::get<1>(video_info) << std::endl;
	std::cout << "Framerate: " << std::get<2>(video_info) << std::endl;
	std::cout << "Number of frames: " << std::get<3>(video_info) << std::endl;

    std::vector<unsigned char*> frames = vid2pixels(vidpath);

	//create an sf texture for the first frame and display it on-screen
	
   

    

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

        sf::RenderWindow window(sf::VideoMode(width, height), "Frame Display");
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


    return 0;
}
