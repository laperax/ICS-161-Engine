#include <iostream>
#include <sstream>
#include <Windows.h>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"
#include "SDL_image.h"
#include "Sprite.h"

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

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
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

	SDL_Window *window = SDL_CreateWindow("Sprite Demo", 800, 100, SCREEN_WIDTH,
		SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	//const std::string resPath = getResourcePath("SpriteDemo");
	const std::string resPath = "Demo\\";

	Sprite* spriteBG = new Sprite(0, 0, resPath + "Document1.txt", renderer);

	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 2;

	Sprite* sprite1 = new Sprite(x, y, resPath + "readme.txt", renderer);
	Sprite* sprite2 = new Sprite(x, y, resPath + "dot.txt", renderer);

	SDL_Event e;
	bool quit = false, left = false, right = false, up = false, down = false;
	std::string spriteDirection = "walk right";
	while (!quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_RIGHT)
				{
					right = true;
				}
				else if (e.key.keysym.sym == SDLK_LEFT)
				{
					left = true;
				}
				else if (e.key.keysym.sym == SDLK_UP)
				{
					up = true;
				}
				else if (e.key.keysym.sym == SDLK_DOWN)
				{
					down = true;
				}
			}

			if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym)
				{
				case SDLK_RIGHT:
					right = false;
					break;
				case SDLK_LEFT:
					left = false;
					break;
				case SDLK_UP:
					up = false;
					break;
				case SDLK_DOWN:
					down = false;
					break;
				}
			}
		}
		if (right)
		{
			sprite2->movex(2);
			sprite1->movex(2);
			spriteDirection = "walk right";
		}
		if (left)
		{
			sprite2->movex(-2);
			sprite1->movex(-2);
			spriteDirection = "walk left";
		}
		if (down)
		{
			sprite2->movey(2);
			sprite1->movey(2);
			spriteDirection = "walk down";
		}
		if (up)
		{
			sprite2->movey(-2);
			sprite1->movey(-2);
			spriteDirection = "walk up";
		}

		//Render the scene
		SDL_RenderClear(renderer);
		spriteBG->show("background", 0);
		sprite1->show(spriteDirection, 1);
		sprite2->show("dot", 0);
		SDL_RenderPresent(renderer);
	}

	cleanup(renderer, window);
	IMG_Quit();
	SDL_Quit();

	SDL_Quit();
	delete spriteBG;
	delete sprite1;
	delete sprite2;

	return 0;
}
