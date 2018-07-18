#pragma once

#include "../../NW_Lib/System/NW_Lib_IScene.h"
#include "MainMenu.h"

namespace Game
{

	//	Credits

	class Credits : public NW_Lib::IScene
	{
	private :
		Button * theButton;
		ButtonType ButtonPressed;
	public :
		Credits();	//	Constructor
		~Credits();	//	Destructor	

		void Initialize();	//	Init
		ButtonType GetPushButtonType();
		void Update();	//	Update

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen
	};

	//	Help

	class Help : public NW_Lib::IScene
	{
	private :
		Button * theButton;
		ButtonType ButtonPressed;
	public :
		Help();	//	Constructor
		~Help();	//	Destructor	

		void Initialize();	//	Init
		ButtonType GetPushButtonType();
		void Update();	//	Update

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen
	};

	//	Level Cleared 

	class LevelCleared : public NW_Lib::IScene
	{
	private :
		int ClearTime;	//	Time taken to clear
		int Score;	//	Score
		int DropletsLeft;	//	Droplets
		int ID;	//	Level ID

		int numOfButtons;	//	Num of buttons 

		float AnimIter;	//	Animation Iteration

		float TopAnimFrame;	//	Top Screen Animation
		float topSegment;	//	For Sprite sheet
		float topTextAlpha;	//	Score fade in

		//	Array for Score
		int ScoreData[4];
		//	Array for time
		int timeData[4];
		//	HighSore
		int HighScoreData[4];
		//	Shortest Time
		int ShortTimeData[4];

		//	Button
		Button * theButton;
		ButtonType ButtonPressed;

	public :
		LevelCleared(int ID, int ClearTime, int DropletsLeft);	//	Constructor
		~LevelCleared();	//	Destructor

		void Initialize();	//	Init
		ButtonType GetPushButtonType();
		void Update();	//	Update

		void NumToSprite();	//	Converts score, time to Sprite 

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen
	};

	//	Confirmation
	class Confirmation : public NW_Lib::IScene
	{
	private : 
		Button * theButton;
		int numOfButton;

		//	percentage for ripple
		float percentage;
		float currentAlpha;
	public :
		Confirmation();
		~Confirmation();

		void Initialize();	//	Init
		void Update();		//	Update
		ButtonType GetPushButtonType();

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen
	};

	//	Menu InGame
	class MenuInGame
	{
	private : 
		Button * theButton;
		int numOfButton;

		//	percentage for ripple
		float percentage;
		float currentAlpha;
	public :
		MenuInGame();
		~MenuInGame();

		void Initialize();	//	Init
		void Update();		//	Update
		ButtonType GetPushButtonType();

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen
	};
}