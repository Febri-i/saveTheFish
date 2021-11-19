#pragma once
#include <SDL.h>
#include <iostream>
#include "Vector2.hpp"

class Stuff
{
	public:
		Stuff(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, bool isDamaging): stuffTex(texture), isDamaging(isDamaging)
		{
			position.x = dest.x;
			position.y = dest.y;
			this->src = src;
			this->dst = dest;
		};
//		~Stuff(){
//			std::cout << "im deleted\n";
//		}
//		void * getSrc(){ return this-> src; };

		void moveX(float x){
			position.x += x;
	//		dst.y += position.y;
		}

		SDL_Rect * getInfo(){ return &this->dst; };

		bool isDamage() { return this->isDamaging; }

		void render(SDL_Renderer* renderer){
			dst.x = round(position.x);

			//std::cout << dst.x << std::endl;
			SDL_RenderCopy(renderer, stuffTex, &src, &dst);

		}

		void checkCollison(SDL_Rect * another){
			if((another->y + another->h > dst.y && another->y + another->h < dst.y + dst.h)){
				std::cout << "Horizontally collide" << std::endl;
			//	if()
			}
		}

	private:
		Vector2<float> position;
		SDL_Texture* stuffTex;
		bool isDamaging;
		SDL_Rect src;
		SDL_Rect dst;
};
