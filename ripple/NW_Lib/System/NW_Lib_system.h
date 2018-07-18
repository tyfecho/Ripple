/**
*	@file	system.h
*
*	@brief	���{�I�ȃV�X�e�����܂Ƃ߂��N���X
*
*	@date	2012/08/01
*
*	@author	�g�c�@����
*/

#pragma once

#include <Windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <d3dx9.h>

#include "../System/NW_Lib_Common.h"

namespace Game
{
	class GameMain;
}

namespace NW_Lib
{
	class Window;
	class DirectX;
	class Light;
	class Timer;

	class DirectXSystem:public Common
	{
		private:
			Window*  m_pWindow;
			DirectX* m_pDirectX;
			Light*   m_pLight;
			Timer*   m_pTimer;

		public:
			DirectXSystem();
			~DirectXSystem();
			void Initialize();
			void Update(Game::GameMain* pGamemain);
	};
}
