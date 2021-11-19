#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


class Window
{
	public:
		Window(int w_width, int w_height, const char* w_title): window(NULL), renderer(NULL)
		{
			this->window = SDL_CreateWindow(w_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, SDL_WINDOW_SHOWN);

			if(window == NULL) std::cout << "FAILED TO CREATE WINDOW" << SDL_GetError() << std::endl;
	
			this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
	
			if(renderer == NULL) std::cout << "FAILED TO CREATE RENDERER" << SDL_GetError() << std::endl;
	
		};

		SDL_Texture* loadMedia(const char* filePath)
		{
			SDL_Texture* texture = NULL; 
			texture = IMG_LoadTexture(this->renderer, filePath);
			if(texture == NULL) std::cout << "Failed to load media: " <<  filePath << SDL_GetError() << std::endl;
			return texture;
		};
		
		void clear(){
			SDL_RenderClear(this->renderer);
		};

		void display(){
			SDL_RenderPresent(this->renderer);
		};

		void destroy(){
			SDL_DestroyWindow(this->window);
		};

		SDL_Renderer* getRenderer(){
			return this->renderer;
		}

	private:
		SDL_Window* window;
		SDL_Renderer* renderer;
};

