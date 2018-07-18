#include "SpriteObject.h"

#include "../../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"

using namespace NW_Lib;

namespace Game
{
	SpriteObject::SpriteObject(float x, float y, float w, float h, float tx, float ty, float tw, float th, bool isButton, float cx, float cy, float angle, float alpha)
	: m_pos(VECTOR2(x, y))
	, m_size(VECTOR2(w, h))
	, m_texPos(VECTOR2(tx, ty))
	, m_texSize(VECTOR2(tw, th))
	, m_isButton(isButton)
	, m_centerPos(VECTOR2(cx, cy))
	, m_angle(angle)
	, m_alpha(alpha)
	{
	}

	SpriteObject::~SpriteObject(void)
	{
	}

	void SpriteObject::Initialize()
	{
	}

	void SpriteObject::Update()
	{
	}

	void SpriteObject::Draw(const TEXTURE* texture)
	{

		SpriteDrawer::Draw(texture,
			m_pos.x+m_centerPos.x, m_pos.y+m_centerPos.y, m_size.x, m_size.y,
			m_texPos.x, m_texPos.y, m_texSize.x, m_texSize.y,
			m_centerPos.x, m_centerPos.y, m_angle, m_alpha
		);
	}

} // namespace Game
