
/**
*	@file	ResourceManager.h
*
*	@brief	���\�[�X�}�l�[�W�����g���ۃN���X
*
*	@date	2013/08/15
*
*	@author	�F���@����
*/

#pragma once

//#include <d3dx9.h>
#include <map>

#include "../Utility/NW_Lib_utility.h"

namespace NW_Lib
{
	using namespace std;

	//�V�[���}�l�[�W���N���X
	template <typename T>

	class ResourceManager
	{

	private:

		//�}�l�[�W��
		map<int , T> m_manager;
		
	public:

		/*=======================================================
			�֐���	�F	ResourceManager
			����	�F	�R���X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		ResourceManager()
		{

		}

		/*=======================================================
			�֐���	�F	~ResourceManager
			����	�F	�f�X�g���N�^
			����	�F	�Ȃ�
			�߂�l	�F�@�Ȃ�
		========================================================*/
		~ResourceManager()
		{
			AllRemove();
		}

		/*=======================================================
			�֐���	�F	Entry
			����	�F	���L�f�[�^�̎擾
			����	�F	�o�^ID
						�I�u�W�F�N�g
			�߂�l	�F�@����
		========================================================*/
		bool Entry(int entryID , T obj)
		{
			if( !m_manager.count(entryID) )
			{
				m_manager.insert( map<int , T>::value_type( entryID , obj));

				return true;
			}

			return false;
		}

		/*=======================================================
			�֐���	�F	Get
			����	�F	�f�[�^�̎擾
			����	�F	�o�^ID
			�߂�l	�F�@�I�u�W�F�N�g
		========================================================*/
		T Get(int ID)
		{
			return m_manager[ID];
		}

		/*=======================================================
			�֐���	�F	Remove
			����	�F	�f�[�^�̏���
			����	�F	�o�^ID
			�߂�l	�F�@����
		========================================================*/
		bool Remove(int ID)
		{
			SafeDelete(m_manager[ID]);
			return m_manager.erase(ID);
		}

		/*=======================================================
			�֐���	�F	AllRemove
			����	�F	�f�[�^�̏���
			����	�F	�o�^ID
			�߂�l	�F�@�I�u�W�F�N�g
		========================================================*/
		void AllRemove()
		{
			if( !m_manager.empty() )
			{
				for( map<int,T>::iterator it = m_manager.begin() ; it != m_manager.end() ; it++ )
				{
					SafeDelete((*it).second);
				}
			}
			if( !m_manager.empty() )
			{
				m_manager.clear();
			}
		}
		
		/*=======================================================
			�֐���	�F	GetSize
			����	�F	�T�C�Y�̎擾
			����	�F	�Ȃ�
			�߂�l	�F�@�o�^�ςݗv�f��
		========================================================*/
		int GetSize()
		{
			return m_manager.size();
		}
		/*=======================================================
			�֐���	�F	GetId
			����	�F	ID�̎擾
			����	�F	�C���f�b�N�X
			�߂�l	�F�@�o�^�ς�ID
		========================================================*/
		int GetId(int index)
		{
			int temp = 0;
			if( !m_manager.empty() )
			{
				for( map<int,T>::iterator it = m_manager.begin() ; it != m_manager.end() ; it++ )
				{
					if( temp == index)
					{
						return it->first;
					}
					temp++;
				}
			}

			return 0;
		}

	};

}