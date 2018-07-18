#pragma once

#include "../../NW_Lib/Utility/NW_Lib_Constant.h"
#include "../../NW_Lib/Graphics/NW_Lib_texture.h"

namespace Game
{

	class SpriteObject
	{
		private:
			VECTOR2 m_pos;
			VECTOR2 m_size;
			VECTOR2 m_texPos;
			VECTOR2 m_texSize;
			bool m_isButton;		// �{�^�������������ǂ���
			VECTOR2 m_centerPos;	// ��]���̒��S
			float m_angle;
			float m_alpha;

		public:
			// �R���X�g���N�^
			SpriteObject(float x, float y, float w, float h, float tx, float ty, float tw, float th, bool isButton=false, float cx=0.0f, float cy=0.0f, float angle=0.0f, float alpha=1.0f);
			// �f�X�g���N�^
			~SpriteObject(void);

			// ������
			void Initialize();
			// �X�V
			void Update();
			// �`��
			void Draw(const TEXTURE* texture);

			// Getter
			VECTOR2 GetPos(){return m_pos;};
			VECTOR2 GetCenterPos() {return VECTOR2(m_pos.x+m_size.x/2, m_pos.y+m_size.y/2);};
			VECTOR2 GetSize(){return m_size;};
			VECTOR2 GetTexPos(){return m_texPos;};
			VECTOR2 GetTexSize(){return m_texSize;};
			// Setter
			void SetPos(VECTOR2 pos){this->m_pos = pos;};
			void SetSize(VECTOR2 size){this->m_size = size;};
			void SetTexPos(VECTOR2 tPos){this->m_texPos = tPos;};
			void SetTexSize(VECTOR2 tSize){this->m_texSize = tSize;};
			void SetAngle(float angle){this->m_angle = angle;};
	};

} // namespace Game

