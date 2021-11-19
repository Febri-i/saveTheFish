#pragma once
#include <SDL.h>
#include <vector>

class UiText{
	public:
		UiText(){};
};

class Ui
{
	public:
		Ui(int x, int y, int w, int h): box{.x = x, .y = y, .w = w, .h = h} {};
		void setColor(SDL_Color color){
			this->color = color;
		}

		void onClick(void(*p_onClickCb)()){
			this->onClickCb = p_onClickCb;
		}
		
		void render(){
			
		}

	private:
		std::vector<UiText> texts;
		SDL_Rect box;
		void(*onClickCb)() = [](){};
		SDL_Color color;
};
