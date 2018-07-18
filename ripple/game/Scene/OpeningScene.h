#pragma once

#include "../../NW_Lib/System/NW_Lib_IScene.h"

namespace Game
{
	class OpeningScene : public NW_Lib::IScene
	{
	private :
		float anim_time;	//	Animation frame
		float fall_height;	//	First set anim falling
		bool explode;
		float explosionTime;	//	duration of explosion animation
	public :
		OpeningScene(){};	//	Constructor
		~OpeningScene();	//	Destructor

		void Initialize();	//	Init
		void Update();	//	Update

		virtual void DrawUpper();	//	Upper Screen

		virtual void DrawLower(){};	//	Lower Screen
	};
};