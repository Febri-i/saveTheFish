#pragma once
#include <iostream>
#include <SDL.h>
#include "Vector2.hpp"

class Fish
{
	public:
		Fish(SDL_Texture* texture, SDL_Rect src, Vector2<int> size, Vector2<int> pos): src(src) ,dst{.x = pos.x, .y = pos.y, .w = size.x, .h = size.y}, fishTexture(texture)
		{
			y = pos.y;
	//		std::cout << dst.x << " " << dst.y << " " << dst.w << " " << dst.h << "\n";
	//		this->src.x = 0;
	//		this->src.y = 0;
	//		this->src.w = this->src.h = 128;
		};

		void chase(Vector2<int> pos){
			target = pos;
//			chasing = true;
		}

//		void stopChase

//		bool isChasing(){ return chasing; }

		float getPos(){ return y; };

		void update(float deltaTime, Vector2<float> direction){
			this->direction = direction;
			//std::cout << deltaTime << std::endl;
			y += direction.y;
			dst.y = round(y) - (dst.h / 2);
		}
		
		void render(SDL_Renderer* renderer){
			SDL_RenderCopyEx(renderer, fishTexture, &src, &dst, std::atan2(direction.y, direction.x) * 180 / 3.1416, NULL, SDL_FLIP_NONE);
		}
	private:
//		bool chasing = false;
		float speed = 0;
		float y;
		SDL_Texture* fishTexture;
		Vector2<float> direction;
		Vector2<int> target;
		SDL_Rect src, dst;
};
