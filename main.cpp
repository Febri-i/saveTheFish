#define KEY e.key.keysym.sym
#include <SDL.h>
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <random>
#include <cstdlib>
#include <string>

#include "includes/Vector2.hpp"
#include "includes/Window.hpp"
#include "includes/CommonGameUtils.hpp"
#include "includes/Fish.hpp"
#include "includes/Player.hpp"

int randomgen(int max, int min = 0) //Pass in range
{
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max); // distribution in range [1, 6]
  return dist6(rng);
}

int SCR_WIDTH = 1024;
int SCR_HEIGHT = 512;

struct Stuff
{
	Vector2<float> position;
	int id;
	float extraY;

	bool isFood, up;
	int angle;
};

int main( int argc, char* args[] )
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
		std::cout << "ERROR::SDL_INIT::SDL_INIT_VIDEO" << SDL_GetError() << std::endl;
	else {
		if(!(IMG_Init(IMG_INIT_PNG)))
			std::cout << "ERROR::IMG_INIT::IMG_INIT_PNG" << SDL_GetError() <<std::endl;		
		if(TTF_Init() != 0)
			std::cout << "ERROR::TTF_INIT" << SDL_GetError() << std::endl;
		if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
         std::cout << "ERROR::SDL_mixer::MIXER_INIT\n" << Mix_GetError() << std::endl;
	}

//	TTF_Init();

	Window window(SCR_WIDTH, SCR_HEIGHT, "Shooting Game");

	int waterHeight = 100;

	SDL_Event e;

	Uint32 last = SDL_GetTicks(), now, deltaTime;

	SDL_Texture* water = window.loadMedia("./texture/water.png");

	SDL_Rect waterSrc{0, 0, 64, 64}, waterDest{0, 100, 64, 64};

	float offsetX = 0;

	int totalWaterInScreen = ceil(SCR_WIDTH / 64) + 1;

	SDL_Texture* trash[4] = {window.loadMedia("./texture/pant.png"), window.loadMedia("./texture/snack.png"), window.loadMedia("./texture/bottle.png"), window.loadMedia("./texture/fishnet.png")};
	SDL_Texture* food[2] = {window.loadMedia("./texture/apple.png"), window.loadMedia("./texture/banana.png")};
	
	SDL_Rect dest{.x = 200, .y = 200, .w = 32, .h = 32}, src{.x = 0, .y = 0, .w = 64, .h = 64};
	
	int jarak = 200;
		
	std::vector<Stuff> stuffs;

	Fish fish(window.loadMedia("./texture/fish.png"), SDL_Rect{.x = 0, .y = 0, .w = 128, .h = 128}, Vector2<int>(48, 48), Vector2<int>(100, SCR_HEIGHT / 2));

	SDL_Rect bg{0, 0, SCR_WIDTH, waterHeight + waterDest.h};

	for(int i = 0; i <= 12; i++){
		int x = SCR_WIDTH + jarak * i + randomgen(jarak / 2);
		int y = randomgen(SCR_HEIGHT - 50, waterHeight + waterDest.h);
		bool isFood = randomgen(1);
		stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(isFood ? 1 : 3), .extraY = 0, .isFood = isFood, .up = (bool)(i % 2), .angle = (int)randomgen(360)});// : stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(3), .extraY = 0, .isFood = false});
	}



	fish.chase(Vector2<int>(stuffs.front().position.x, stuffs.front().position.y));

	bool running = true, dead = false, playing = true;

	Vector2<int> mousePos;
	
	SDL_Texture* playerTex = window.loadMedia("./texture/player.png");


	//SFX

	Mix_Chunk *trashSFX = NULL, *pointSFX = NULL, *foodSFX = NULL, *drownSFX = NULL;

	trashSFX = Mix_LoadWAV("./sfx/eatTrash.wav");
	pointSFX = Mix_LoadWAV("./sfx/point.wav");

	drownSFX = Mix_LoadWAV("./sfx/drown.wav");
	
	if(!(trashSFX || pointSFX || drownSFX))
		std::cout << "ERROR::FAILED_LOAD_SFX\nPlease check if there is missing sfx file\n";
	///SFX////



	Player player(playerTex, 100, 100);

	
	TTF_Font * font = TTF_OpenFont("./fonts/Poppins-ExtraBold.ttf", 25);
	TTF_Font * deadWarn = TTF_OpenFont("./fonts/Poppins-ExtraBold.ttf", 100);

	TTF_SetFontOutline(deadWarn, 5);

	int point = 0;
	std::string pointstr = "0";

	SDL_Rect score{SCR_WIDTH - 50, 25, 0, 0};

	float stuffSPeed = 1;


	SDL_SetRenderDrawBlendMode(window.getRenderer(), SDL_BLENDMODE_BLEND);

	SDL_Rect warning;

	for(Stuff &stuff : stuffs){
		std::cout << stuff.isFood << " " << stuff.id << std::endl;
	}

	bool alreadyPlayed = false;
	while(running){
		now = SDL_GetTicks();
		deltaTime = now - last;
		last = now;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT)
				running = false;
			if(e.type == SDL_MOUSEBUTTONDOWN){
				if(!playing && dead){
					dead = false;
					playing = true;
					player.reset();
					stuffs.clear();
						for(int i = 0; i <= 12; i++){
							int x = SCR_WIDTH + jarak * i + randomgen(jarak / 2);
							int y = randomgen(SCR_HEIGHT - 50, waterHeight + waterDest.h);
							bool isFood = randomgen(1);
							stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(isFood ? 1 : 3), .extraY = 0, .isFood = isFood, .up = (bool)(i % 2), .angle = (int)randomgen(360)});// : stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(3), .extraY = 0, .isFood = false});
						}
					fish.chase(Vector2<int>(stuffs.front().position.x, stuffs.front().position.y));
					
					alreadyPlayed = false;


				}
				else
					player.setSwim(true);
			}
			else if(e.type == SDL_MOUSEBUTTONUP)
				player.setSwim(false);
			
		
		}
		SDL_SetRenderDrawColor(window.getRenderer(), 0, 113, 200, 255);
		window.clear();
		SDL_SetRenderDrawColor(window.getRenderer(), 255, 255, 255, 255);

		SDL_RenderFillRect(window.getRenderer(), &bg);

		if(playing){
			fish.update(deltaTime / 3.5f, Vector2<float>(stuffs.front().position.x - 100, stuffs.front().position.y + 32 - fish.getPos()).normalize());
			player.update(deltaTime, mousePos);
	
		Vector2<float> playerBound = player.getBound();
		for(auto stuff = stuffs.begin(); stuff != stuffs.end(); stuff++){		
			if(playerBound.x > stuff->position.x && playerBound.x < stuff->position.x + dest.w && playerBound.y > stuff->position.y && playerBound.y < stuff->position.y + dest.h){	
				int x = stuffs.back().position.x + jarak + randomgen(jarak / 2);
				int y = randomgen(SCR_HEIGHT - 50, waterHeight + waterDest.h);
				bool isFood = randomgen(1);
				if(stuff->isFood){
					if(point > 0)
						point--;
				}else{
					point++;
					Mix_PlayChannel(-1, pointSFX, 0);
				}
				pointstr = std::to_string(point);
				stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(isFood ? 1 : 3), .extraY = 0, .isFood = isFood, .up = !stuffs.back().up, .angle = (int)randomgen(360)});
				stuffs.erase(stuff);
			}
		}

		if(stuffs.front().position.x < 132){
			if(!stuffs.front().isFood){
				playing = stuffs.front().isFood;
				dead = !stuffs.front().isFood;
				Mix_PlayChannel(-1, trashSFX, 0);
			}
			int x = stuffs.back().position.x + jarak + randomgen(jarak / 2);	
			int y = randomgen(SCR_HEIGHT - 50, waterHeight + waterDest.h);
			bool isFood = randomgen(1);
			stuffs.push_back(Stuff{.position=Vector2<float>(x, y), .id = randomgen(isFood ? 1 : 3), .extraY = 0, .isFood = isFood, .up = !stuffs.back().up, .angle = (int)randomgen(360)});
			stuffs.erase(stuffs.begin());
		}		
			
			for(Stuff &stuff : stuffs){
				stuff.extraY = Approach((stuff.up ? -10.f : 10.f), stuff.extraY, deltaTime / 20.f);
				if(stuff.extraY >= 10.f)
					stuff.up = true;
				else if(stuff.extraY <= -10.f)
					stuff.up = false;
				stuff.position.x -= deltaTime / 7.5f;

				
			}

			offsetX += deltaTime / 10.f;
			if(offsetX >= waterDest.w)
				offsetX = 0;
			
		}
		
		for(int i = 0; i < totalWaterInScreen; i++){
			waterDest.x = i * waterDest.w - floor(offsetX);
			SDL_RenderCopy(window.getRenderer(), water, &waterSrc, &waterDest);
		}
		
		fish.render(window.getRenderer());
		
		SDL_Surface* texto = TTF_RenderText_Solid(font, pointstr.c_str(), SDL_Color{0, 0, 0});
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(window.getRenderer(), texto);
		SDL_QueryTexture(textTexture, NULL, NULL, &score.w, &score.h);

		SDL_RenderCopy(window.getRenderer(), textTexture, NULL, &score);
		SDL_FreeSurface(texto);
		SDL_DestroyTexture(textTexture);

		player.render(window.getRenderer());
		for(auto stuff = stuffs.begin(); stuff != stuffs.end(); stuff++){

				dest.x = round(stuff->position.x);
   	      dest.y = round(stuff->position.y + stuff->extraY);
   	   	SDL_RenderCopyEx(window.getRenderer(), (stuff->isFood ? food[stuff->id] : trash[stuff->id]),&src, &dest, stuff->angle, NULL, SDL_FLIP_NONE);

		}
		
		if(player.isDeads()){
			if(!alreadyPlayed){
				Mix_PlayChannel(-1, drownSFX, 0);
				alreadyPlayed = true;
			}
			dead = player.isDeads();
			playing = !player.isDeads();
		}

		if(!playing && dead){
			SDL_SetRenderDrawColor(window.getRenderer(), 0, 0, 0, 100);
			SDL_RenderFillRect(window.getRenderer(), NULL);
		};

		if(!playing && dead){
			TTF_SetFontOutline(deadWarn, 5);
			SDL_Surface* outlineBg = TTF_RenderText_Solid(deadWarn,  "Game Over",SDL_Color{0, 0, 0});
			TTF_SetFontOutline(deadWarn, 0);
			SDL_Surface* actualText = TTF_RenderText_Solid(deadWarn, "Game Over",SDL_Color{255, 0, 0});
			SDL_Texture* outlineBgT = SDL_CreateTextureFromSurface(window.getRenderer(), outlineBg);
			SDL_Texture* actualTextT = SDL_CreateTextureFromSurface(window.getRenderer(), actualText);
			SDL_QueryTexture(outlineBgT, NULL, NULL, &warning.w, &warning.h);
			warning.x = 512 - (warning.w / 2);
			warning.y = 256 - (warning.h / 2);
			SDL_RenderCopy(window.getRenderer(), outlineBgT, NULL, &warning);
			SDL_QueryTexture(actualTextT, NULL, NULL, &warning.w, &warning.h);
			warning.x = 512 - (warning.w / 2);
			warning.y = 256 - (warning.h / 2);
			SDL_RenderCopy(window.getRenderer(), actualTextT, NULL, &warning);
			SDL_FreeSurface(outlineBg);
			SDL_FreeSurface(actualText);
			SDL_DestroyTexture(actualTextT);
			SDL_DestroyTexture(outlineBgT);
		}


		window.display();
	}
	window.destroy();
	for(int i = 0; i < 4; i++)
		SDL_DestroyTexture(trash[i]);
	Mix_FreeChunk(foodSFX);
	Mix_FreeChunk(trashSFX);
	Mix_FreeChunk(drownSFX);
	Mix_FreeChunk(pointSFX);
	SDL_DestroyTexture(food[0]);
	SDL_DestroyTexture(food[1]);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;

}

