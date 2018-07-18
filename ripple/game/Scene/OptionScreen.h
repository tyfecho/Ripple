#pragma once
#include "MainMenu.h"
#include "../../NW_Lib/System/NW_Lib_IScene.h"

namespace Game
{
	class OptionScreen : public NW_Lib::IScene
	{
	private :

		//	Buttons
		Button * OptionButton;
		int numOfButton;
		ButtonType ButtonPressed;

		//	Sliders
		Button * Slider;
		int numOfSlider;

		//	Value
		int sfxSlider, bgmSlider;

	public :

		OptionScreen();		//	Constructor

		~OptionScreen();	//	Destructor

		void Initialize();

		ButtonType GetPushButtonType();

		void Update();

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower();	//	Lower Screen

		void DetectSliderCollision();	//	Slide collision
	};
}