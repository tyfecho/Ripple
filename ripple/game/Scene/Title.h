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
			// コンストラクタ
			Title();
			// デストラクタ
			~Title();
			// 初期化
			void Initialize();
			// 更新
			void Update();

			// 上画面描画
			virtual void DrawUpper();
			
			// 下画面描画
			virtual void DrawLower();
	};
}


