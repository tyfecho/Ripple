#pragma once

#include "../../NW_Lib/System/NW_Lib_IScene.h"

namespace Game
{
	enum ButtonType
	{
		PLAY,	
		OPTION,
		ENG,
		JAP,
		HELP,
		CREDITS,
		SFX,
		BGM,
		MENU,
		BACK,
		NEXT,
		RESTART,
		YES,
		NO,
		NONE
	};

	class Button
	{
	private :
		float pos_x, pos_y;	//	Co-Ordinates
		float width, height;	//	Size of button
		ButtonType CurrentButtonType;	//	What is the button type
		float rotation, rateOfRotation;
	public :
		Button(void){};	//	Constructor
		~Button(){};	//	Destructor

		//	Initialise
		void Init(float pos_x,float pos_y,float width,float height,ButtonType CurrentButtonType,int rotation, int rateOfRotation)
		{
			this->pos_x = pos_x;
			this->pos_y = pos_y;
			this->width = width;
			this->height = height;

			this->CurrentButtonType = CurrentButtonType;	//	Start Button
			this->rotation = rotation;
			this->rateOfRotation = rateOfRotation;
		}

		//	Get Values
		float GetPosX() { return this->pos_x; };
		float GetPosY() { return this->pos_y; };
		

		float GetWidth() { return this->width; };
		float GetHeight() { return this->height; };

		float GetRotation() { return this->rotation; };
		float GetRateOfRotation() { return this->rateOfRotation; };

		ButtonType GetCurrentButtonType() { return this->CurrentButtonType; };

		//	Set Values
		void SetPosX(float pos_x) { this->pos_x = pos_x; };
		void SetPosY(float pos_y) { this->pos_y = pos_y; };

		void SetWidth(float width) { this->width =  width; };
		void SetHeight(float height) { this->height = height; };

		void SetRotation(float rotation) { this->rotation = rotation; };
		void SetRateOfRotation(float rateOfRotation) { this->rateOfRotation = rateOfRotation; };

		
		bool DetectCollision(void);
		bool DetectCollisionTouchRelease(void);
		

	};

	class MainMenu : public NW_Lib::IScene
	{
	private :
		int numOfButtons;	//	Number of Buttons
		ButtonType ButtonPressed;	//	Which button was pressed
		ButtonType LanButtonPressed;	//	For language

		int CurrentLanguage;
		Button * MenuButton;	//	Pointer to Buttons

	public :
		MainMenu();	//	Constructor

		~MainMenu();	//	Destructor
		
		void Initialize();	//	Init

		void Update();	//	Update

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen

		void Finalize();	//	Final

	};
}