/**
*	@file	ModelControler.cpp
*
*	@brief	���f���`��V�X�e���N���X
*
*	@date	2013/09/03
*
*	@author	�g�c�@����
*	@author	�F���@����
*/

#pragma once

#include "NW_Lib_ModelControler.h"

#include "NW_Lib_ModelManager.h"

namespace NW_Lib
{

	/// ���f���̕`��
	void ModelControler::Draw(int handle)
	{
		ModelManager::GetInstance().Draw(DrawingScreenID,handle);
	}

	/// ���f���̈ʒu�̐ݒ�
	void ModelControler::SetPosision(int handle,VECTOR3 position)
	{
		ModelManager::GetInstance().SetPosision(handle,position);
	}

	/// ���f���̉�]�̐ݒ�
	void ModelControler::SetRotate(int handle,VECTOR3 rotate)
	{
		ModelManager::GetInstance().SetRotate(handle,rotate);
	}
	
	/// ���f���̃T�C�Y�̐ݒ�
	void ModelControler::SetScale(int handle,VECTOR3 scale)
	{
		ModelManager::GetInstance().SetScale(handle,scale);
	}

	/// ���f���ɍs���ݒ�
	void ModelControler::SetWorldMatrix(int handle,MATRIX* matrix)
	{
		ModelManager::GetInstance().SetMatrix(handle,matrix);
	}
}
