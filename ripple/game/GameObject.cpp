/*
* GameObject.cpp
*/

/***************************************************
*	File includes
***************************************************/

#include "GameObject.h"

#include "../NW_Lib/Graphics/NW_Lib_SpriteDrawer.h"
#include "../NW_Lib/Graphics/NW_Lib_TextureManager.h"
#include "../NW_Lib/Graphics/NW_Lib_texture.h"

#include "../NW_Lib/Utility/NW_Lib_HardwareDefine.h"

#include <math.h>

#include "../NW_Lib/Utility/NW_Lib_DebugFontManager.h"

using namespace NW_Lib;

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/***************************************************
	*	Game object class functions
	****************************************************/
	//		Stores basic information such as position, velocity, direction and size
	//		Built-in functions to handle texture information and sprite animation 

	/*
	*	コンストラクタ
	*/
	GameObject::GameObject(void)
	{
		m_ObjectID = 0;

		m_Active = true;

		m_TexInfo.id = -1;
		m_Alpha = 1.f;

		m_FlipHorizontal = false;
		m_FlipVertical = false;

		m_Position = VECTOR2(0.f, 0.f);
		m_Velocity = VECTOR2(0.f, 0.f);
		m_Direction = VECTOR2(0.f, 1.f);
		m_Size = VECTOR2(0.f, 0.f);
		m_Scale = VECTOR2(1.f, 1.f);
		
		m_CollisionRadius = 0.f;
		m_CollisionShape = ECOLLISIONSHAPE_NONE;
	}

	/*
	*	デストラクタ
	*/
	GameObject::~GameObject(void)
	{
		m_Vertex.clear();
	}

	/*
	*	Sets the texture information
	*/
	void GameObject::SetTexture(int id, int widthPerFrame, int heightPerFrame, int offsetX, int offsetY, int maxFrame)
	{
		m_TexInfo.id = id;
		m_TexInfo.width = TextureManager::Get(id)->GetWidth();
		m_TexInfo.height = TextureManager::Get(id)->GetHeight();

		if (widthPerFrame == 0)
			m_TexInfo.widthPerFrame = m_TexInfo.width;
		else
			m_TexInfo.widthPerFrame = widthPerFrame;

		if (heightPerFrame == 0)
			m_TexInfo.heightPerFrame = m_TexInfo.height;
		else
			m_TexInfo.heightPerFrame = heightPerFrame;

		m_TexInfo.offsetX = offsetX;
		m_TexInfo.offsetY = offsetY;

		m_TexInfo.maxFrame = maxFrame;
		m_TexInfo.framePerRow = m_TexInfo.width / m_TexInfo.widthPerFrame;
		m_TexInfo.framePerCol = (int)ceil((float)maxFrame/m_TexInfo.framePerRow);

		m_TexInfo.currentFrame = 0.f;
		m_TexInfo.animationSpeed = 1.f;
		m_TexInfo.animationState = 0;
	}

	/*
	*	Calculate a collision radius automatically based on the size values.
	*	Inner circle determines whether to calculate a collision circle that is within the rect, or one that wraps around the rect
	*	If the size is not a square, diffSizeCalculationMethod determines whether to use the smaller, average or larger value of the two for calculation
	*	-1 = use the smaller value, 0 = use the average value, 1 = use the larger value
	*/
	void GameObject::CalcCollisionRadius(bool innerCircle, int diffSizeCalculationMethod)
	{
		float temp;

		//If the x and y components are the same
		if (m_Size.x == m_Size.y)
			//Set temp value to the x-component (Either one works as both are same)
			temp = m_Size.x;

		//Else if the x and y components are not the same
		else
		{
			//Determine the temp value depending on the method specified
			switch (diffSizeCalculationMethod)
			{
				//Use the smaller value
				case -1:	temp = (m_Size.x > m_Size.y) ? m_Size.y : m_Size.x;
							break;
				//Use the average value
				case 0:		temp = (m_Size.x + m_Size.y)*0.5f;
							break;
				//Use the larger value
				case 1:		temp = (m_Size.x > m_Size.y) ? m_Size.x : m_Size.y;
							break;
			}
		}

		if (innerCircle)
			m_CollisionRadius = temp;
		else
			m_CollisionRadius = sqrt(temp*temp*2);

		m_CollisionRadius *= 0.5f;
	}

	/*
	*
	*/
	VECTOR2 GameObject::GetBoundingSize()
	{
		float size;
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	size = m_CollisionRadius*2;
											return VECTOR2(size, size);
			case ECOLLISIONSHAPE_AABB:
			case ECOLLISIONSHAPE_TRIANGLE:	return m_Size;
			case ECOLLISIONSHAPE_OBB:		size = max(m_Size.x, m_Size.y);
											return VECTOR2(size, size);
			case ECOLLISIONSHAPE_POINT:
			case ECOLLISIONSHAPE_NONE:		return VECTOR2(0.f, 0.f);
		}
	}

	/*
	*	Checks for collision based off the collision shape of the object(s)
	*/
	bool GameObject::CollideObject(GameObject* obj)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	switch (obj->GetCollisionShape())
											{
												case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleCircle(this->m_Position, this->m_CollisionRadius, obj->m_Position, obj->GetCollisionRadius());
												case ECOLLISIONSHAPE_AABB:		return CollideCircleAABB(this->m_Position, this->m_CollisionRadius, obj->m_Position, obj->m_Size);
												case ECOLLISIONSHAPE_OBB:		return CollideCircleOBB(this->m_Position, this->m_CollisionRadius, obj->m_Position, obj->m_Size, obj->m_Direction);
												case ECOLLISIONSHAPE_TRIANGLE:	return CollideCircleTriangle(this->m_Position, this->m_CollisionRadius, obj->m_Vertex.get(0), obj->m_Vertex.get(1), obj->m_Vertex.get(2));
												case ECOLLISIONSHAPE_POINT:		return CollideCirclePoint(this->m_Position, this->m_CollisionRadius, obj->m_Position);
												case ECOLLISIONSHAPE_NONE:		return false;
											}
											break;
			case ECOLLISIONSHAPE_AABB:		switch (obj->GetCollisionShape())
											{
												case ECOLLISIONSHAPE_CIRCLE:	return CollideAABBCircle(this->m_Position, this->m_Size, obj->m_Position, obj->GetCollisionRadius());
												case ECOLLISIONSHAPE_AABB:		return CollideAABBAABB(this->m_Position, this->m_Size, obj->m_Position, obj->m_Size);
												case ECOLLISIONSHAPE_OBB:		return CollideOBBOBB(this->m_Position, this->m_Size, VECTOR2(0.f, 1.f), obj->m_Position, obj->m_Size, obj->m_Direction);
												case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideAABBTriangle(this->m_Position, this->m_Size, obj->m_Vertex.get(0), obj->m_Vertex.get(1), obj->m_Vertex.get(2));
												case ECOLLISIONSHAPE_POINT:		return CollideAABBPoint(this->m_Position, this->m_Size, obj->m_Position);
												case ECOLLISIONSHAPE_NONE:		return false;
											}
											break;
			case ECOLLISIONSHAPE_OBB:		switch (obj->GetCollisionShape())
											{
												case ECOLLISIONSHAPE_CIRCLE:	return CollideOBBCircle(this->m_Position, this->m_Size, this->m_Direction, obj->m_Position, obj->GetCollisionRadius());
												case ECOLLISIONSHAPE_AABB:		return CollideOBBOBB(this->m_Position, this->m_Size, this->m_Direction, obj->m_Position, obj->m_Size, VECTOR2(0.f, 1.f));
												case ECOLLISIONSHAPE_OBB:		return CollideOBBOBB(this->m_Position, this->m_Size, this->m_Direction, obj->m_Position, obj->m_Size, obj->m_Direction);
												case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideOBBTriangle(this->m_Position, this->m_Size, this->m_Direction, obj->m_Vertex.get(0), obj->m_Vertex.get(1), obj->m_Vertex.get(2));
												case ECOLLISIONSHAPE_POINT:		return CollideOBBPoint(this->m_Position, this->m_Size, this->m_Direction, obj->m_Position);
												case ECOLLISIONSHAPE_NONE:		return false;
											}
											break;
			case ECOLLISIONSHAPE_TRIANGLE:	switch (obj->GetCollisionShape())
											{
												case ECOLLISIONSHAPE_CIRCLE:	return CollideTriangleCircle(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), obj->m_Position, obj->GetCollisionRadius());
												case ECOLLISIONSHAPE_AABB:		return false;//return CollideTriangleAABB(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), obj->m_Position, obj->m_Size);
												case ECOLLISIONSHAPE_OBB:		return false;//return CollideTriangleOBB(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), obj->m_Position, obj->m_Size, obj->m_Direction);
												case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideTriangleTriangle(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), obj->m_Vertex.get(0), obj->m_Vertex.get(1), obj->m_Vertex.get(2));
												case ECOLLISIONSHAPE_POINT:		return CollideTrianglePoint(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), obj->m_Position);
												case ECOLLISIONSHAPE_NONE:		return false;
											}
											break;
			case ECOLLISIONSHAPE_POINT:		switch (obj->GetCollisionShape())
											{
												case ECOLLISIONSHAPE_CIRCLE:	return CollidePointCircle(this->m_Position, obj->m_Position, obj->GetCollisionRadius());
												case ECOLLISIONSHAPE_AABB:		return CollidePointAABB(this->m_Position, obj->m_Position, obj->m_Size);
												case ECOLLISIONSHAPE_OBB:		return CollidePointOBB(this->m_Position, obj->m_Position, obj->m_Size, obj->m_Direction);
												case ECOLLISIONSHAPE_TRIANGLE:	return CollidePointTriangle(this->m_Position, obj->m_Vertex.get(0), obj->m_Vertex.get(1), obj->m_Vertex.get(2));
												case ECOLLISIONSHAPE_POINT:		return (this->m_Position == obj->m_Position);
												case ECOLLISIONSHAPE_NONE:		return false;
											}
											break;
			case ECOLLISIONSHAPE_NONE:		return false;
											
		}
		return false;
	}

	bool GameObject::CollideCircle (VECTOR2 circ_pos, float circ_radius)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleCircle(this->m_Position, this->m_CollisionRadius, circ_pos, circ_radius);
			case ECOLLISIONSHAPE_AABB:		return CollideAABBCircle(this->m_Position, this->m_Size, circ_pos, circ_radius);
			case ECOLLISIONSHAPE_OBB:		return CollideOBBCircle(this->m_Position, this->m_Size, this->m_Direction, circ_pos, circ_radius);
			case ECOLLISIONSHAPE_TRIANGLE:	return CollideTriangleCircle(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), circ_pos, circ_radius);
			case ECOLLISIONSHAPE_POINT:		return CollidePointCircle(this->m_Position, circ_pos, circ_radius);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	bool GameObject::CollideAABB (VECTOR2 rect_pos, VECTOR2 rect_size)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleAABB(this->m_Position, this->m_CollisionRadius, rect_pos, rect_size);
			case ECOLLISIONSHAPE_AABB:		return CollideAABBAABB(this->m_Position, this->m_Size, rect_pos, rect_size);
			case ECOLLISIONSHAPE_OBB:		return CollideOBBOBB(this->m_Position, this->m_Size, this->m_Direction, rect_pos, rect_size, VECTOR2(0.f, 1.f));
			case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideTriangleAABB(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), rect_pos, rect_size);
			case ECOLLISIONSHAPE_POINT:		return CollidePointAABB(this->m_Position, rect_pos, rect_size);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	bool GameObject::CollideOBB (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleOBB(this->m_Position, this->m_CollisionRadius, rect_pos, rect_size, rect_dir);
			case ECOLLISIONSHAPE_AABB:		return CollideOBBOBB(this->m_Position, this->m_Size, VECTOR2(0.f, 1.f), rect_pos, rect_size, rect_dir);
			case ECOLLISIONSHAPE_OBB:		return CollideOBBOBB(this->m_Position, this->m_Size, this->m_Direction, rect_pos, rect_size, rect_dir);
			case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideTriangleOBB(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), rect_pos, rect_size, rect_dir);
			case ECOLLISIONSHAPE_POINT:		return CollidePointOBB(this->m_Position, rect_pos, rect_size, rect_dir);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	bool GameObject::CollideTriangle (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleTriangle(this->m_Position, this->m_CollisionRadius, tri_p1, tri_p2, tri_p3);
			case ECOLLISIONSHAPE_AABB:		return false;//return CollideAABBTriangle(this->m_Position, this->m_Size, tri_p1, tri_p2, tri_p3);
			case ECOLLISIONSHAPE_OBB:		return false;//return CollideOBBTriangle(this->m_Position, this->m_Size, this->m_Direction, tri_p1, tri_p2, tri_p3);
			case ECOLLISIONSHAPE_TRIANGLE:	return false;//return CollideTriangleTriangle(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), tri_p1, tri_p2, tri_p3);
			case ECOLLISIONSHAPE_POINT:		return CollidePointTriangle(this->m_Position, tri_p1, tri_p2, tri_p3);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	bool GameObject::CollidePoint (VECTOR2 point_pos)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCirclePoint(this->m_Position, this->m_CollisionRadius, point_pos);
			case ECOLLISIONSHAPE_AABB:		return CollideAABBPoint(this->m_Position, this->m_Size, point_pos);
			case ECOLLISIONSHAPE_OBB:		return CollideOBBPoint(this->m_Position, this->m_Size, this->m_Direction, point_pos);;
			case ECOLLISIONSHAPE_TRIANGLE:	return CollideTrianglePoint(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), point_pos);
			case ECOLLISIONSHAPE_POINT:		return (this->m_Position == point_pos);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	bool GameObject::CollideLine (VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return CollideCircleLine(this->m_Position, this->m_CollisionRadius, line_start, line_end, collisionPoint, collisionNormal);
			case ECOLLISIONSHAPE_AABB:		return CollideAABBLine(this->m_Position, this->m_Size, line_start, line_end, collisionPoint, collisionNormal);
			case ECOLLISIONSHAPE_OBB:		return CollideOBBLine(this->m_Position, this->m_Size, this->m_Direction, line_start, line_end, collisionPoint, collisionNormal);;
			case ECOLLISIONSHAPE_TRIANGLE:	return CollideTriangleLine(this->m_Vertex.get(0), this->m_Vertex.get(1), this->m_Vertex.get(2), line_start, line_end, collisionPoint, collisionNormal);
			case ECOLLISIONSHAPE_POINT:		return false;//return CollidePointLine(this->m_Position, line_start, line_end);
			case ECOLLISIONSHAPE_NONE:		return false;
		}
	}

	/*
	*	Gets the closest point on the current object from another point
	*/
	VECTOR2 GameObject::GetClosestPoint (VECTOR2 point, bool clampToEdge)
	{
		switch (m_CollisionShape)
		{
			case ECOLLISIONSHAPE_CIRCLE:	return GetClosestPointOnCircle(point, m_Position, m_CollisionRadius, clampToEdge);
			case ECOLLISIONSHAPE_AABB:		return GetClosestPointOnAABB(point, m_Position, m_Size, clampToEdge);
			case ECOLLISIONSHAPE_OBB:		return GetClosestPointOnOBB(point, m_Position, m_Size, m_Direction, clampToEdge);
			case ECOLLISIONSHAPE_TRIANGLE:	return GetClosestPointOnTriangle(point, m_Vertex.get(0), m_Vertex.get(1), m_Vertex.get(2), clampToEdge);
			case ECOLLISIONSHAPE_POINT:
			case ECOLLISIONSHAPE_NONE:		return m_Position;
			
		}
	}

	/*
	*	Plays a sprite animation with the specified speed and number of times (-1 to loop) 
	*	If relative is set to true, the animation will not be reset and animation state will be compounded to the previous state
	*/
	void GameObject::PlayAnimation(float animationSpeed, int animationState, bool relative)
	{
		m_TexInfo.animationSpeed = animationSpeed;
		if (relative)
		{
			m_TexInfo.animationState += animationState;
		}
		else
		{
			m_TexInfo.currentFrame = 0.f;
			m_TexInfo.animationState = animationState;
		}
	}

	/*
	*	Stops the sprite animation. Resets both the animation state and speed
	*/
	void GameObject::StopAnimation()
	{
		m_TexInfo.currentFrame = 0.f;
		m_TexInfo.animationSpeed = 0.f;
		m_TexInfo.animationState = 0;
	}

	/*
	*	更新
	*/
	void GameObject::Update(float deltaTime)
	{
		m_Position += m_Velocity*deltaTime;
		
		/*
		* Update sprite animation
		*/

		//If the animation state is not set to stop
		if (m_TexInfo.animationState != 0)
		{
			//Increase the current frame counter
			m_TexInfo.currentFrame += m_TexInfo.animationSpeed*deltaTime;

			//If animation is updating forward
			if (m_TexInfo.animationSpeed > 0.f)
			{
				//Loop as long as current animation frame is more than the max
				while (m_TexInfo.currentFrame >= m_TexInfo.maxFrame)
				{
					//If animation is still playing
					if (m_TexInfo.animationState == -1 || --m_TexInfo.animationState > 0)
						//Decrease the current frame counter by the max
						m_TexInfo.currentFrame -= m_TexInfo.maxFrame;
					//Else if animation has come to a stop
					else
						//Set the current frame to 0
						m_TexInfo.currentFrame = 0;
				}
				//end loop
			}
			//endif forward animation
			//Else if animation is updating backward
			else if (m_TexInfo.animationSpeed < 0.f)
			{
				//Loop as long as current animation frame is less than the min
				while (m_TexInfo.currentFrame < 0.f)
				{
					//If animation is still playing
					if (m_TexInfo.animationState == -1 || --m_TexInfo.animationState > 0)
						//Increase the current frame counter by the max
						m_TexInfo.currentFrame += m_TexInfo.maxFrame;
					//Else if animation has come to a stop
					else
						//Set the current frame to 0
						m_TexInfo.currentFrame = 0;
				}
				//end loop
			}
			//endif backward animation
		}
		//endif animation not set to stop
	}

	/*
	*	描画
	*/
	void GameObject::Render(float offsetX, float offsetY, bool SwapScreen)
	{
		if (m_TexInfo.id == -1)
			return;

		VECTOR2 tCoord;
		if (m_TexInfo.framePerRow == 1)
		{
			tCoord.x = m_TexInfo.offsetX + m_TexInfo.widthPerFrame  * ((int)m_TexInfo.currentFrame % m_TexInfo.framePerRow);
			tCoord.y = m_TexInfo.offsetY + m_TexInfo.heightPerFrame * ((int)m_TexInfo.currentFrame / m_TexInfo.framePerRow);
		}
		else
		{
			int frameX = (int)m_TexInfo.currentFrame + ceil(((float)m_TexInfo.offsetX / m_TexInfo.widthPerFrame));
			tCoord.x = m_TexInfo.widthPerFrame  * (frameX % m_TexInfo.framePerRow) - (m_TexInfo.offsetX % m_TexInfo.widthPerFrame);
			tCoord.y = m_TexInfo.heightPerFrame * ((int)m_TexInfo.currentFrame / m_TexInfo.framePerRow) + m_TexInfo.offsetY;
		}
		
		VECTOR2 tSize (m_TexInfo.widthPerFrame-0.01f, m_TexInfo.heightPerFrame-0.01f);
		if (m_FlipHorizontal)
		{
			tCoord.x += m_TexInfo.widthPerFrame-0.01f;
			tSize.x *= -1;
		}
		if (m_FlipVertical ^ SwapScreen)
		{
			tCoord.y += m_TexInfo.heightPerFrame-0.01f;
			tSize.y *= -1;
		}

		VECTOR2 scaledSize = m_Size;
		scaledSize.x *= m_Scale.x;
		scaledSize.y *= m_Scale.y;

		VECTOR2 pos = m_Position;
		if (SwapScreen)
			pos.y = TOTAL_SCREEN_HEIGHT - m_Position.y;
		pos.x += offsetX;
		pos.y += offsetY;

		float angle = Vec2AngleDeg(m_Direction);
		if (SwapScreen)
			angle *= -1.f;

		SpriteDrawer::Draw(	TextureManager::Get(m_TexInfo.id),
							pos.x, pos.y,
							scaledSize.x, scaledSize.y,
							tCoord.x, tCoord.y, tSize.x, tSize.y,
							scaledSize.x*0.5f, scaledSize.y*0.5f,
							angle,
							m_Alpha
						);
	}

	/***************************************************
	*	Collision functions
	****************************************************/
	//		Collision check functions between circle, AABB, OBB, triangle, point and line
	//		Line-collision functions can also optionally return the point and normal of collision

	/*
	*	Collision with circle
	*/

	bool CollideCircleCircle (VECTOR2 circ1_pos, float circ1_radius, VECTOR2 circ2_pos, float circ2_radius)
	{
		float radSum = circ1_radius+circ2_radius;
		VECTOR2 tempVec = circ1_pos - circ2_pos;
		if (Vec2LengthSq(tempVec) <= radSum*radSum)
			return true;
		return false;
	}

	bool CollideAABBCircle (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 circ_pos, float circ_radius)
	{
		VECTOR2 closestPoint = GetClosestPointOnAABB(circ_pos, rect_pos, rect_size, false);
		return CollidePointCircle(closestPoint, circ_pos, circ_radius);
	}

	bool CollideOBBCircle (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 circ_pos, float circ_radius)
	{
		VECTOR2 closestPoint = GetClosestPointOnOBB(circ_pos, rect_pos, rect_size, rect_dir, false);
		return CollidePointCircle(closestPoint, circ_pos, circ_radius);
	}

	bool CollideTriangleCircle (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 circ_pos, float circ_radius)
	{
		VECTOR2 closestPoint = GetClosestPointOnTriangle(circ_pos, tri_p1, tri_p2, tri_p3, false);
		return CollidePointCircle(closestPoint, circ_pos, circ_radius);
	}

	bool CollidePointCircle (VECTOR2 point_pos, VECTOR2 circ_pos, float circ_radius)
	{
		if (Vec2LengthSq(point_pos - circ_pos) <= circ_radius*circ_radius)
			return true;
		return false;
	}

	bool CollideLineCircle (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 circ_pos, float circ_radius, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		VECTOR2 line_vec = line_end - line_start;

		if (CollidePointCircle(line_start, circ_pos, circ_radius))
		{
			if (collisionPoint)
				*collisionPoint = line_start;
			
			if (collisionNormal)
				*collisionNormal = Vec2Normalize(line_start - circ_pos);

			return true;
		}

		float a = Vec2Dot(line_vec,line_vec);
		float b = Vec2Dot(line_vec,((line_start-circ_pos)*2.0f));
		float c = Vec2Dot(circ_pos,circ_pos) + Vec2Dot(line_start,line_start) - Vec2Dot(line_start,circ_pos)*2.0f - circ_radius*circ_radius;
		float D = b*b + (-4.0f)*a*c;

		// If ray can not intersect then stop
		if (D < 0)
			return false;
		D=sqrtf(D);

		// Ray can intersect the circle, solve the closer hitpoint
		float t = (-0.5f)*(b+D)/a;

		VECTOR2 hitpoint;
		if (t > 0.0f)
		{
			hitpoint = line_start + (line_vec*t);
			if ( Vec2LengthSq(hitpoint - line_start) > Vec2LengthSq(line_vec) )
				return false;
		}
		else
			return false;

		if (collisionPoint)
			*collisionPoint = hitpoint;

		if (collisionNormal)
			*collisionNormal = Vec2Normalize(hitpoint - circ_pos);

		return true;
	}

	/*
	*	Collision with AABB
	*/

	bool CollideCircleAABB (VECTOR2 circ_pos, float circ_radius, VECTOR2 rect_pos, VECTOR2 rect_size)
	{
		return CollideAABBCircle(rect_pos, rect_size, circ_pos, circ_radius);
	}

	bool CollideAABBAABB (VECTOR2 rect1_pos, VECTOR2 rect1_size, VECTOR2 rect2_pos, VECTOR2 rect2_size)
	{
		if (   (rect1_pos.x - rect1_size.x*0.5f) > (rect2_pos.x + rect2_size.x*0.5f)	//Rect 1's left end is right of Rect 2's right end
			|| (rect1_pos.x + rect1_size.x*0.5f) < (rect2_pos.x - rect2_size.x*0.5f)	//Rect 1's right end is left of Rect 2's left end
			|| (rect1_pos.y - rect1_size.y*0.5f) > (rect2_pos.y + rect2_size.y*0.5f)	//Rect 1's top end is below Rect 2's bottom end
			|| (rect1_pos.y + rect1_size.y*0.5f) < (rect2_pos.y - rect2_size.y*0.5f)	//Rect 1's bottom end is above Rect 2's top end
			)
			return false;
		return true;
	}

	bool CollidePointAABB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size)
	{
		if (   point_pos.x >= rect_pos.x - rect_size.x*0.5f
			&& point_pos.x <= rect_pos.x + rect_size.x*0.5f
			&& point_pos.y >= rect_pos.y - rect_size.y*0.5f
			&& point_pos.y <= rect_pos.y + rect_size.y*0.5f 
			)
			return true;
		return false;
	}

	bool CollideLineAABB (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		VECTOR2 line_dir = Vec2Normalize(line_end - line_start);

		VECTOR2 rect_min = rect_pos - rect_size*0.5f;
		VECTOR2 rect_max = rect_pos + rect_size*0.5f;

		VECTOR2 t1 = (rect_min - line_start);
		VECTOR2 t2 = (rect_max - line_start);
		t1.x /= line_dir.x;
		t1.y /= line_dir.y;
		t2.x /= line_dir.x;
		t2.y /= line_dir.y;

		float tmax = min(max(t1.x, t2.x), max(t1.y, t2.y));

		// if tmax < 0, ray (line) is intersecting AABB, but AABB is behind
		if (tmax < 0)
		{
			//dist = tmax;
			return false;
		}

		float tmin = max(min(t1.x, t2.x), min(t1.y, t2.y));

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax)
		{
			//dist = tmax;
			return false;
		}

		float dist = tmin;

		if (dist*dist >= Vec2LengthSq(line_end - line_start))
			return false;

		if (collisionPoint)
			*collisionPoint = line_start + line_dir*dist;

		if (collisionNormal)
		{
			VECTOR2 hitpoint;
			if (collisionPoint)
				hitpoint = *collisionPoint;
			else
				hitpoint = line_start + line_dir*dist;

			collisionNormal->x = (hitpoint.x <= rect_min.x) ? -1.f : (hitpoint.x >= rect_max.x) ? 1.f : 0.f;
			collisionNormal->y = (hitpoint.y <= rect_min.y) ? -1.f : (hitpoint.y >= rect_max.y) ? 1.f : 0.f;
			*collisionNormal = Vec2Normalize(*collisionNormal);
		}

		return true;
	}

	/*
	*	Collision with OBB
	*/

	bool CollideCircleOBB (VECTOR2 circ_pos, float circ_radius, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir)
	{
		return CollideOBBCircle(rect_pos, rect_size, rect_dir, circ_pos, circ_radius);
	}

	bool CollideOBBOBB (VECTOR2 rect1_pos, VECTOR2 rect1_size, VECTOR2 rect1_dir, VECTOR2 rect2_pos, VECTOR2 rect2_size, VECTOR2 rect2_dir)
	{
		return true;
	}

	bool CollidePointOBB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir)
	{
		float rad = Vec2AngleRad(rect_dir);
		VECTOR2 p = Vec2RotateRad(point_pos - rect_pos, rad);
		return CollidePointAABB(p, VECTOR2(0.f, 0.f), rect_size);
	}

	bool CollideLineOBB (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
#ifdef _DirectX
		float rad = Vec2AngleRad(rect_dir);
#endif
#ifdef _3DS
		float rad = -Vec2AngleRad(rect_dir);
#endif
		VECTOR2 p1 = Vec2RotateRad(line_start - rect_pos, rad);
		VECTOR2 p2 = Vec2RotateRad(line_end - rect_pos, rad);
		if (CollideLineAABB(p1, p2, VECTOR2(0.f, 0.f), rect_size, collisionPoint, collisionNormal))
		{
			if (collisionPoint)
			{
				*collisionPoint = Vec2RotateRad(*collisionPoint, -rad);
				*collisionPoint += rect_pos;
			}
			if (collisionNormal)
				*collisionNormal = Vec2RotateRad(*collisionNormal, -rad);

			return true;
		}

		return false;
	}

	/*
	*	Collision with triangle
	*/
	bool CollideCircleTriangle (VECTOR2 circ_pos, float circ_radius, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3)
	{
		return CollideTriangleCircle(tri_p1, tri_p2, tri_p3, circ_pos, circ_radius);
	}

	bool CollidePointTriangle (VECTOR2 point_pos, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3)
	{
		bool b1, b2, b3;

		b1 = sign(point_pos, tri_p1, tri_p2) < 0.0f;
		b2 = sign(point_pos, tri_p2, tri_p3) < 0.0f;
		b3 = sign(point_pos, tri_p3, tri_p1) < 0.0f;

		return ((b1 == b2) && (b2 == b3));
	}

	bool CollideLineTriangle (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		//Check collision with each sides of the triangle
		//Collision normal is calculated under the assumption that the points provided are in clockwise order
		if (CollideLineLine(tri_p1, tri_p2, line_start, line_end, collisionPoint))
		{
			if (collisionNormal)
			{
				VECTOR2 temp = Vec2Normalize(tri_p1 - tri_p2);
				collisionNormal->x = temp.y;
				collisionNormal->y = -temp.x;
			}
			return true;
		}
		else if (CollideLineLine(tri_p2, tri_p3, line_start, line_end, collisionPoint))
		{
			if (collisionNormal)
			{
				VECTOR2 temp = Vec2Normalize(tri_p2 - tri_p3);
				collisionNormal->x = temp.y;
				collisionNormal->y = -temp.x;
			}

			return true;
		}
		else if (CollideLineLine(tri_p3, tri_p1, line_start, line_end, collisionPoint))
		{
			if (collisionNormal)
			{
				VECTOR2 temp = Vec2Normalize(tri_p3 - tri_p1);
				collisionNormal->x = temp.y;
				collisionNormal->y = -temp.x;
			}
			return true;
		}

		return false;
	}

	/*
	*	Collision with point
	*/

	bool CollideCirclePoint (VECTOR2 circ_pos, float circ_radius, VECTOR2 point_pos)
	{
		return CollidePointCircle(point_pos, circ_pos, circ_radius);
	}

	bool CollideAABBPoint (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 point_pos)
	{
		return CollidePointAABB(point_pos, rect_pos, rect_size);
	}

	bool CollideOBBPoint (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 point_pos)
	{
		return CollidePointOBB(point_pos, rect_pos, rect_size, rect_dir);
	}

	bool CollideTrianglePoint (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 point_pos)
	{
		return CollidePointTriangle(point_pos, tri_p1, tri_p2, tri_p3);
	}

	/*
	*	Collision with line
	*/
	bool CollideCircleLine (VECTOR2 circ_pos, float circ_radius, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		return CollideLineCircle(line_start, line_end, circ_pos, circ_radius, collisionPoint, collisionNormal);
	}

	bool CollideAABBLine (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		return CollideLineAABB(line_start, line_end, rect_pos, rect_size, collisionPoint, collisionNormal);
	}

	bool CollideOBBLine (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		return CollideLineOBB(line_start, line_end, rect_pos, rect_size, rect_dir, collisionPoint, collisionNormal);
	}

	bool CollideTriangleLine (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint, VECTOR2* collisionNormal)
	{
		return CollideLineTriangle(line_start, line_end, tri_p1, tri_p2, tri_p3, collisionPoint, collisionNormal);
	}

	bool CollideLineLine (VECTOR2 line1_start, VECTOR2 line1_end, VECTOR2 line2_start, VECTOR2 line2_end, VECTOR2* collisionPoint)
	{
		VECTOR2 AB = line1_end - line1_start;
		VECTOR2 CD = line2_end - line2_start;
    
		float d = Vec2PerpDot(CD,AB);
		if(d == 0)			// parallel lines
			return false;
        
		VECTOR2 AC = line2_start - line1_start;
		float k = Vec2PerpDot(CD,AC) / d;
		if(k <  0)      return false;
		if(k >= 1)      return false;
    
		float m = Vec2PerpDot(AB,AC) / d;
		if(m <  0)      return false;
		if(m >= 1)      return false;
    
		if (collisionPoint)
		{
			m -= 0.0001f;
			*collisionPoint = line2_start + CD*m;
		}

		return true;
	}

	/***************************************************
	*	2D vector functions
	****************************************************/
	//		Additional 2D vector functions

	/*
	*	Get the (squared) length of the vector
	*/
	float Vec2LengthSq (VECTOR2 vec)
	{
		return vec.x*vec.x + vec.y*vec.y;
	}

	float Vec2Length (VECTOR2 vec)
	{
		return sqrt(Vec2LengthSq(vec));
	}

	/*
	*	Get the angle of a vector relative to (0, -1)
	*	+ve anti-clockwise and -ve clockwise
	*/
	float Vec2AngleRad(VECTOR2 vec)
	{
#ifdef _DirectX
		 return atan2(vec.x, vec.y);
#endif
#ifdef _3DS
		 return atan2(-vec.x, vec.y);
#endif
	}

	float Vec2AngleDeg(VECTOR2 vec)
	{
		return Vec2AngleRad(vec) * RADTODEG;
	}

	/*
	*	Get the angle between 2 vectors
	*/
	float Vec2AngleRad (VECTOR2 vec1, VECTOR2 vec2)
	{
		return acos( clamp(Vec2Dot(Vec2Normalize(vec1), Vec2Normalize(vec2)), 0.f, 1.f) );
	}

	float Vec2AngleDeg (VECTOR2 vec1, VECTOR2 vec2)
	{
		return Vec2AngleRad(vec1, vec2) * RADTODEG;
	}

	/*
	*	Get the dot product of vec1 and vec2
	*/
	float Vec2Dot (VECTOR2 vec1, VECTOR2 vec2)
	{
		return vec1.x*vec2.x + vec1.y*vec2.y;
	}

	/*
	*	Get the perpendicular dot product - a modification of the dot product where 'a' is replaced with the perpendicular vector rotated to the left by 90 degrees
	*/
	float Vec2PerpDot (VECTOR2 vec1, VECTOR2 vec2) 
	{
		return (vec1.y*vec2.x) - (vec1.x*vec2.y);
	}

	/*
	*	Get the unit vector of the vector
	*/
	VECTOR2 Vec2Normalize (VECTOR2 vec)
	{
		return vec / Vec2Length(vec);
	}

	/*
	*	Reflect an incindence vector, vec1, about the reflection vector, vec2
	*/
	VECTOR2 Vec2Reflect (VECTOR2 vec1, VECTOR2 vec2)
	{
		return vec1 - 2*vec2*Vec2Dot(vec1,vec2);
	}

	/*
	*	Rotate a vector by a specified angle
	*/
	VECTOR2 Vec2RotateRad (VECTOR2 vec, float rad, bool fast_trigo)
	{
		float sinR = (fast_trigo) ? fast_sin_rad(rad) : sin(rad);
		float cosR = (fast_trigo) ? fast_cos_rad(rad) : cos(rad);
		return VECTOR2(cosR*vec.x - sinR*vec.y
					  ,sinR*vec.x + cosR*vec.y
					  );
	}

	VECTOR2 Vec2RotateDeg (VECTOR2 vec, float deg, bool fast_trigo)
	{
		float sinD = (fast_trigo) ? fast_sin_deg(deg) : sin(deg*DEGTORAD);
		float cosD = (fast_trigo) ? fast_cos_deg(deg) : cos(deg*DEGTORAD);
		return VECTOR2(cosD*vec.x - sinD*vec.y
					  ,sinD*vec.x + cosD*vec.y
					  );
	}

	/*
	*	Get a vector perpendicular to the given vector in the clockwise or counter-clockwise direction
	*/
	VECTOR2 Vec2PerpCW (VECTOR2 vec)
	{
		return VECTOR2(vec.y, -vec.x);
	}

	VECTOR2 Vec2PerpCCW (VECTOR2 vec)
	{
		return VECTOR2(-vec.y, vec.x);
	}

	/*
	*	Get the projection component/vector of vec1 on vec2
	*/
	float Vec2ProjComponent (VECTOR2 vec1, VECTOR2 vec2)
	{
		return Vec2Dot(vec1, Vec2Normalize(vec2));
	}

	VECTOR2 Vec2ProjVector (VECTOR2 vec1, VECTOR2 vec2)
	{
		return Vec2ProjComponent(vec1, vec2) * vec2;
	}

	/***************************************************
	*	Miscellaneous functions
	****************************************************/
	//		Additional functions for various uses

	/*
	*
	*/
	bool EpsilonEqual(float a, float b)
	{
		return (fabsf(a - b) < FLT_EPSILON);
	}

	/*
	*	Limit a number within the min and max value
	*/
	int clamp (int num, int min, int max) 
	{
		return (num < min) ? min : (num > max) ? max : num;
	}

	float clamp (float num, float min, float max) 
	{
		return (num < min) ? min : (num > max) ? max : num;
	}

	/*
	*	Fast trigometric functions using a look-up table
	*/
	float fast_sin_rad (float rad)
	{
		return fast_sin_deg((int)(rad*RADTODEG));
	}

	float fast_sin_deg (int deg)
	{
		deg %= 360;
		if (deg < 0)
			deg += 360;
		int quadrant = deg / 90;
		deg %= 90;

		switch (quadrant)
		{
			case 0: return sinTable[deg];
			case 1: return sinTable[90 - deg];
			case 2: return -sinTable[deg];
			case 3: return -sinTable[90 - deg];
		}
	}

	float fast_cos_rad (float rad)
	{
		return fast_sin_deg((int)(rad*RADTODEG) + 90);
	}

	float fast_cos_deg (int deg)
	{
		return fast_sin_deg(deg + 90);
	}

	/*
	*	Determine which side of the line is the point on
	*/
	float sign (VECTOR2 point, VECTOR2 line_start, VECTOR2 line_end)
	{
		return (point.x - line_end.x) * (line_start.y - line_end.y) - (line_start.x - line_end.x) * (point.y - line_end.y);
	}

	/*
	*	Get the point on a geometry that is nearest to a specified point
	*/
	VECTOR2 GetClosestPointOnCircle (VECTOR2 point_pos, VECTOR2 circ_pos, float circ_radius, bool clampToEdge)
	{
		VECTOR2 vec = point_pos - circ_pos;
		float dist = Vec2Length(vec);
		return circ_pos + vec * ( ((clampToEdge) ? circ_radius : clamp(dist, 0.f, circ_radius)) / dist);
	}

	VECTOR2 GetClosestPointOnAABB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, bool clampToEdge)
	{
		VECTOR2 rect_min = rect_pos - rect_size*0.5f;
		VECTOR2 rect_max = rect_pos + rect_size*0.5f;
		VECTOR2 point;
		if (clampToEdge && CollidePointAABB(point_pos, rect_pos, rect_size))
		{
			VECTOR2 vec = Vec2Normalize(point_pos - rect_pos);
			if (Vec2LengthSq(vec) == 0.f)
				vec.x = -1.f;
			point = GetClosestPointOnAABB(rect_pos + vec*max(rect_size.x, rect_size.y), rect_pos, rect_size);
		}
		else
		{
			point.x = clamp(point_pos.x, rect_min.x, rect_max.x);
			point.y = clamp(point_pos.y, rect_min.y, rect_max.y);
		}

		return point;
	}

	VECTOR2 GetClosestPointOnOBB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, bool clampToEdge)
	{
		VECTOR2 half_rect_size = rect_size * 0.5f;
		VECTOR2 d = point_pos - rect_pos;
		VECTOR2 perpDir = VECTOR2(-rect_dir.y, rect_dir.x);
		float dist = Vec2Dot(d, perpDir);
		dist = clamp(dist, -half_rect_size.x, half_rect_size.x);
		
		VECTOR2 q = rect_pos + perpDir*dist;
		dist = Vec2Dot(d, rect_dir);
		dist = clamp(dist, -half_rect_size.y, half_rect_size.y);
		return q + rect_dir*dist;
	}

	VECTOR2 GetClosestPointOnTriangle (VECTOR2 point_pos, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, bool clampToEdge)
	{
		if (!clampToEdge && CollidePointTriangle(point_pos, tri_p1, tri_p2, tri_p3))
			return point_pos;

		VECTOR2 point[3];
		point[0] = GetClosestPointOnLine(point_pos, tri_p1, tri_p2);
		point[1] = GetClosestPointOnLine(point_pos, tri_p2, tri_p3);
		point[2] = GetClosestPointOnLine(point_pos, tri_p3, tri_p1);

		float distSq[3];
		for (int i = 0; i < 3; ++ i)
			distSq[i] = Vec2LengthSq(point_pos - point[i]);

		return (distSq[0] < distSq[1]) ? (distSq[0] < distSq[2]) ? point[0] : point[2] : (distSq[1] < distSq[2]) ? point[1] : point[2];
	}

	VECTOR2 GetClosestPointOnLine (VECTOR2 point_pos, VECTOR2 line_start, VECTOR2 line_end, bool clampToLine)
	{
		VECTOR2 AP = point_pos - line_start;
		VECTOR2 AB = line_end - line_start;

		float t = Vec2Dot(AP, AB) / Vec2LengthSq(AB);

		if (clampToLine)
			t = clamp(t, 0.f, 1.f);

		return line_start + AB*t;
	}
}