// C++ and SDL header files
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"
#include "SDL_image.h"
#include <string>
#include <fstream>

// Custom engine header files
#include "Sprite/Sprite.h"
#include "Json/json.h"
#include "Camera/Camera.h"

// Engine-wide constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
	std::ostringstream errMsg;
	errMsg << " error: " << SDL_GetError() << std::endl;
	OutputDebugString(errMsg.str().c_str());
}

Json::Value loadLevel(std::string filename){
	std::ifstream ifs(filename);
	std::string json((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	Json::Value root;
	Json::Reader reader;
	bool parsed = reader.parse(json.c_str(), root, false);
	if (!parsed){
		std::cout << "Failed to load level" << std::endl;
	}
	return root;
}


int main(int argc, char **argv){

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::ostringstream debugMsg;
		debugMsg << "SDL_Init Error: " << SDL_GetError() << std::endl;
		OutputDebugString(debugMsg.str().c_str());
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("ICS 161 Game Engine", 800, 100, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	Json::Value level = loadLevel("test.json");
	const std::string resPath = "assets\\";
	Sprite* spriteBG = new Sprite(0, 0, resPath + "Document1.txt", renderer);

	Camera camera = Camera(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_Event e;
	bool quit = false;
	while (!quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
				case SDLK_w:
					camera.move(0, -5);
					break;
				case SDLK_d:
					camera.move(5, 0);
					break;
				case SDLK_s:
					camera.move(0, 5);
					break;
				case SDLK_a:
					camera.move(-5, 0);
					break;
				}
			}
		}
		
		//Render the scene
		SDL_RenderClear(renderer);
		camera.render(0, 0, spriteBG, "background");
		SDL_RenderPresent(renderer);
		std::cout << camera.getX() << ":" << camera.getY() << std::endl;
	}

	cleanup(renderer, window);
	IMG_Quit();
	SDL_Quit();

	SDL_Quit();

	return 0;
}
