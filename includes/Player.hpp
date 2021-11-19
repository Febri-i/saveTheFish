#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include "Vector2.hpp"
#include <math.h>
#include "CommonGameUtils.hpp"


class Player
{
	public:
		Player(SDL_Texture* texture, int x, int y)
		{
			playerTex = texture;
			position.x = x;
			position.y = y;
		};

		void setSwim(bool s){
			onSwim = s;
		}

		void reset(){
			isDead = false;
			health = 100.f;
		}

		void update(float dt, Vector2<int> mousePos){
			//std::cout << dt << std::endl;
			if(onSwim)
				direction = Vector2<float>(mousePos.x - (position.x + (dst.w / 2)), mousePos.y - (position.y + (dst.h / 2))).normalize();
			else {
				float angle = std::atan2(direction.y, direction.x);
				float approach = Approach(angle > 1.5708f ? 3.1416 : -1.5708f, angle, dt / 50.f);
				direction = Vector2<float>(cos(approach), sin(approach));
			}			

			if(lastFrame < 1.f / maxframe)
				lastFrame += dt / 100.f;
			else{
				currentFrame++;
				src.x = currentFrame * src.w;
				lastFrame = 0;
			}
			if(currentFrame > maxframe){
				currentFrame = 0;
				src.x = 0;
			};

			if(!( ( mousePos.x >= position.x &&  mousePos.x <= position.x + (dst.w)) && (mousePos.y >= position.y && mousePos.y <= position.y + dst.h ) ) || !onSwim)
				position += direction * (dt * 0.35f);
			
			if(position.y <= 100){
				position.y = 100;
				if(health < 100)
					health += dt / 25.f;
				else
					health = 100;
			}else{
				if(health > 0)
					health -= dt / 100;
				else{
					health = 0;
					isDead = true;
				}
			}
				
			healthBarPercent.w = floor((health / 100.f) * 120);

			dst.x = position.x;
			dst.y = position.y;
		}

		void render(SDL_Renderer* renderer){
			SDL_RenderCopyEx(renderer, playerTex, &src, &dst, std::atan2(direction.y, direction.x) * 180.f / 3.1416, NULL, SDL_FLIP_NONE);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &healthBar);
			if(health){
				SDL_SetRenderDrawColor(renderer, 255, 255, 25, 255);
				SDL_RenderFillRect(renderer, &healthBarPercent);
			}
		}

		Vector2<float> getBound(){ return Vector2<float>((dst.x + (48)), (dst.y + 24)); };

		bool isDeads(){
			return isDead;
		}


	private:
		float lastFrame;
//		float speed = 0.25;
		bool onSwim = false;
		bool isDead = false;
		float health = 100;
		int currentFrame, maxframe = 19;
		SDL_Texture* playerTex;
		Vector2<float> position, direction;
		SDL_Rect src{0, 0, 128, 64}, dst{200, 200, 96, 48}, healthBar{32, 32, 128, 32}, healthBarPercent{36, 36, 122,24};
};
