/**
*	@file	LightControler.cpp
*
*	@brief	ライトを操作するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#include "NW_Lib_LightControler.h"


namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   ライトの位置を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   x 新しく設定する視点のX座標
		 * 
		 * @param   y 新しく設定する視点のX座標
		 * 
		 * @param   z 新しく設定する視点のX座標
		 */
		void LightControler::SetPosition(DrawingScreen drawingscreen,float x,float y,float z)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetPos(x,y,z);
		}
		/**
		 * @brief   ライトの位置を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param pos			新しく設定する視点
		 */
		void LightControler::SetPosition(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetPos(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   ライトの向きを設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   x 新しく設定する視点のX座標
		 * 
		 * @param   y 新しく設定する視点のX座標
		 * 
		 * @param   z 新しく設定する視点のX座標
		 */
		void LightControler::SetDirection(DrawingScreen drawingscreen,float x,float y,float z)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetDirection(x,y,z);
		}
		/**
		 * @brief   ライトの向きを設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param pos			新しく設定する視点
		 */
		void LightControler::SetDirection(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetDirection(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   ライトの色を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   _r 赤色の要素
		 * 
		 * @param   _g 緑色の要素
		 * 
		 * @param   _b 青色の要素
		 */
		void LightControler::SetColor(DrawingScreen drawingscreen,float _r, float _g, float _b)
		{
			LightManager::GetInstance().GetLight(drawingscreen)->SetColor(_r,_g,_b);
		}
	}
}

