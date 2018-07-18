/**
*	@file	CameraControler.cpp
*
*	@brief	カメラを操作するクラス
*
*	@date	2013/09/19
*
*	@author	吉田　将也
*	@author	皆光　健太
*/
#include "NW_Lib_CameraControler.h"

namespace NW_Lib
{
	namespace System
	{
		/**
		 * @brief   カメラの視点を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   x 新しく設定する視点のX座標
		 * 
		 * @param   y 新しく設定する視点のX座標
		 * 
		 * @param   z 新しく設定する視点のX座標
		 */
		void CameraControler::SetPosition(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraPos(x,y,z);
		}
		/**
		 * @brief   カメラの視点を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param pos			新しく設定する視点
		 */
		void CameraControler::SetPosition(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraPos(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   カメラの注視点を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   x 新しく設定する注視点のX座標
		 * 
		 * @param   y 新しく設定する注視点のX座標
		 * 
		 * @param   z 新しく設定する注視点のX座標
		 */
		void CameraControler::SetTarget(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(x,y,z);
		}
		/**
		 * @brief   カメラの注視点を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param pos			新しく設定する注視点
		 */
		void CameraControler::SetTarget(DrawingScreen drawingscreen,VECTOR3 pos)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(pos.x,pos.y,pos.z);
		}

		/**
		 * @brief   カメラの上方向ベクトルを設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param   x 新しく設定する上方向ベクトルのX方向
		 * 
		 * @param   y 新しく設定する上方向ベクトルのY方向
		 * 
		 * @param   z 新しく設定する上方向ベクトルのZ方向
		 */
		void CameraControler::SetUpVec(DrawingScreen drawingscreen,float x,float y,float z)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetVecUp(x,y,z);
		}
		/**
		 * @brief   カメラの注視点を設定
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param vec			新しく設定する注視点
		 */
		void CameraControler::SetUpVec(DrawingScreen drawingscreen,VECTOR3 vec)
		{
			CameraManager::GetInstance().GetCamera(drawingscreen)->SetCameraTarget(vec.x,vec.y,vec.z);
		}

		/**
		 * @brief   正射影か透視射影の切り替え
		 *
		 * @param drawingscreen 上画面か下画面かの指定
		 *
		 * @param cameramode	正射影化透視射影かを設定
		 */
		void CameraControler::SetMode(DrawingScreen drawingscreen,MODE cameramode)
		{
			if(drawingscreen == UPPER_SCREEN_DRAWING){
				if(cameramode == MODE_PERSPECTIVE){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetPerspective();
				}else if(cameramode == MODE_ORTHO){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetOrtho(400.0f,240.0f);
				}
			}else if(drawingscreen == LOWER_SCREEN_DRAWING){
				if(cameramode == MODE_PERSPECTIVE){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetPerspective();
				}else if(cameramode == MODE_ORTHO){
					CameraManager::GetInstance().GetCamera(drawingscreen)->SetOrtho(320.0f,240.0f);
				}
			}
		}
	}
}
