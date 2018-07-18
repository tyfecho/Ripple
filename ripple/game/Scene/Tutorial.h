#pragma once

#include "../LinkedList.h"

namespace Game
{
	class Units
	{
	private :
		float pos_x, pos_y;	//	Position
		float tex_x, tex_y;
		float width, height;	//	Height and Width
		int iter, screen;	//	1 for TOP, 2 for BOTTOM
		bool animated;	//	is it animated
		int frame;	//	if animated
		int entryNum;	//	Texture Entry
		float currentFrame;

	public :

		Units(float pos_x,float pos_y,float tex_x,float tex_y,float width,float height,int iter,int screen,int entryNum,bool animate=false,int frame=0);	//	Constructor
		~Units();	//	Destructor

		//	Actions
		void Render();
		void Update();

		//	Get Values
		float GetPosX() { return this->pos_x; };
		float GetPosY() { return this->pos_y; };
		float GetTexX() { return this->tex_x; };
		float GetTexY() { return this->tex_y; };
		float GetWidth() { return this->width; };
		float GetHeight() { return this->height; };
		int GetIter() { return this->iter; };
		int GetScreen() { return this->screen; };
		bool GetAnimated() { return this->animated; };
		int GetFrame() { return this->frame; };
	};

	class Tutorial 
	{
	private :
		bool isThereOne;
		int id;
		LinkedList<Units*> theUnits;
		float currentIter;
		int maxIter;

	public : 
		Tutorial();	//	Constructor
		~Tutorial();	//	Destructor

		//	Initialise
		void Init(int id);

		//	Get Value
		bool getIsThereOne() { return this->isThereOne; };
		int GetID() { return this->id; };

		//	Set Value
		void SetIsThereOne(bool isThereOne) { this->isThereOne = isThereOne; };

		void RenderUpper();

		void RenderLower();

		void Update();

	};
}
