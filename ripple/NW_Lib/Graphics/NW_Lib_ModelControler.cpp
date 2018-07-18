/**
*	@file	ModelControler.cpp
*
*	@brief	モデル描画システムクラス
*
*	@date	2013/09/03
*
*	@author	吉田　将也
*	@author	皆光　健太
*/

#pragma once

#include "NW_Lib_ModelControler.h"

#include "NW_Lib_ModelManager.h"

namespace NW_Lib
{

	/// モデルの描画
	void ModelControler::Draw(int handle)
	{
		ModelManager::GetInstance().Draw(DrawingScreenID,handle);
	}

	/// モデルの位置の設定
	void ModelControler::SetPosision(int handle,VECTOR3 position)
	{
		ModelManager::GetInstance().SetPosision(handle,position);
	}

	/// モデルの回転の設定
	void ModelControler::SetRotate(int handle,VECTOR3 rotate)
	{
		ModelManager::GetInstance().SetRotate(handle,rotate);
	}
	
	/// モデルのサイズの設定
	void ModelControler::SetScale(int handle,VECTOR3 scale)
	{
		ModelManager::GetInstance().SetScale(handle,scale);
	}

	/// モデルに行列を設定
	void ModelControler::SetWorldMatrix(int handle,MATRIX* matrix)
	{
		ModelManager::GetInstance().SetMatrix(handle,matrix);
	}
}
