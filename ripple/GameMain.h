#pragma once

#include <Windows.h>
#include "NW_Lib/System/NW_Lib_SceneManager.h"


#include "NW_Lib/System/NW_Lib_Camera.h"


#include <fstream>
#include "NW_Lib/System/NW_Lib_Common.h"

namespace NW_Lib
{
	class Sprite3D;
}

namespace Game
{
	class ParticleEffect;

	class GameMain : public NW_Lib::Common
	{
		private:
			// �X���b�h�̃n���h��
			HANDLE hTh;
			// �X���b�hID
			UINT thID;

			bool m_LoadFlg;
			bool m_IsDrawingLogo;
			bool m_IsUpdate;

		public:
			// �R���X�g���N�^
			GameMain();
			// �f�X�g���N�^
			~GameMain();

			// �X�V
			void Update();
			
			// ���ʂ̕`��
			void DrawUpper();

			// ����ʂ̕`��
			void DrawLower();

			static unsigned __stdcall LoadThread(void* v);

			void AllLoad();

	};
}


