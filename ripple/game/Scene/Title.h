#pragma once

#include "../../NW_Lib/System/NW_Lib_IScene.h"

namespace NW_Lib
{

	namespace System
	{
		class FileStream;
	}
}

namespace Game
{
	class Title : public NW_Lib::IScene
	{
		private:
			bool isModel;

			NW_Lib::System::FileStream* m_ifs;

		public:
			// �R���X�g���N�^
			Title();
			// �f�X�g���N�^
			~Title();
			// ������
			void Initialize();
			// �X�V
			void Update();

			// ���ʕ`��
			virtual void DrawUpper();
			
			// ����ʕ`��
			virtual void DrawLower();
	};
}


