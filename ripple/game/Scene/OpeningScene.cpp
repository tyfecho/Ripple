#include "OpeningScene.h"
#include "MainMenu.h"

//#include <Windows.h>
//#include "../../NW_Lib/System/NW_Lib_Common.h"
#include "../../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../../NW_Lib/Graphics/NW_Lib_BillboardDrawer.h"
#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../../NW_Lib/System/NW_Lib_SceneManager.h"
#include "../../NW_Lib/Sound/NW_Lib_Sound.h"

// Sound Data
#ifdef _DirectX
	//#include SoundData
	#include "../../works_romfiles/sound/RippleSoundOther.h"
#endif
#ifdef _3DS
	#include "../../NW_Lib/Graphics/NW_Lib_ResTextureManager.h"
	//#include SoundData
	// ƒTƒEƒ“ƒh
#endif

using namespace NW_Lib;

namespace Game
{

	OpeningScene::~OpeningScene()
	{
	}

	void OpeningScene::Initialize()
	{
		TextureManager::Entry(0,"LogoScreen");

		//	Falling height
		this->fall_height = 1.0f;
		//	Which frame to use
		this->anim_time = 1.0f;
		this->explode = false;
		this->explosionTime = 3.0f;
	}

	void OpeningScene::Update()
	{
		//	Normal Logo 
		float height = 80.0f;
		if ( this->fall_height < height )
			this->fall_height += height/10.0f;	//	Falling
		else
		{
			if ( !explode )
			{
				if ( this->anim_time < 2.5f )	//	stay on the ground
					this->anim_time++;	//	increase time frame 
				else
				{
					this->explode = true;
					this->anim_time = 0.0f;
				}
			}
			if ( explode )	//	explode
			{
				if ( this->anim_time < this->explosionTime )	//	explode time
					this->anim_time++;
				else
				{
					if ( this->anim_time < 7.0f )	//	stay there
						this->anim_time++;
					else
						System::SceneManager::GetInstance().ChangeScene(new MainMenu());
				}
			}
		}
	};

	void OpeningScene::DrawUpper()
	{
		//	Black Screen
		SpriteDrawer::Draw(TextureManager::Get(0),0.0f,0.0f,400.0f,240.0f,0.0f,0.0f,400.0f,240.0f);

		//	Normal Logo 
		if ( this->explode && this->anim_time < this->explosionTime )
			SpriteDrawer::Draw(TextureManager::Get(0),100.0f,this->fall_height,200.0f,80.0f,0.0f,240.0f,200.0f,80.0f);
		else
			SpriteDrawer::Draw(TextureManager::Get(0),100.0f,this->fall_height,200.0f,80.0f,200.0f,240.0f,200.0f,80.0f);
	}
}