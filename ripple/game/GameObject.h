/*
* GameObject.h
*	Basic class and functionalities for game objects that are commonly used
*	-Texture Info Struct
*		Stores basic texture and sprite animation information
*	-Collision Shape Enumerator
*		Type of collision check to use for the object
*	-2D GameObject Class
*		Stores basic information such as position, velocity, direction and size
*		Built-in functions to handle texture information and sprite animation 
*	-Collision Functions
*		Collision check functions between circle, AABB, OBB, triangle, point and line
*		Line-collision functions can also optionally return the point and normal of collision
*	-2D Vector Functions
*		Additional 2D vector functions
*	-Miscellaneous Functions
*		Additional functions for various uses
*/

#pragma once

/***************************************************
*	File includes
***************************************************/

#include "../NW_Lib/Utility/NW_Lib_Constant.h"
#include "LinkedList.h"

/***************************************************
*	Macro Definitions
***************************************************/

#define TOTAL_SCREEN_HEIGHT	(LOWER_SCREEN_HEIGHT + UPPER_SCREEN_HEIGHT)
#define SCREEN_WIDTH_DIFF	(UPPER_SCREEN_WIDTH - LOWER_SCREEN_WIDTH)
#define GAME_SCREEN_WIDTH	LOWER_SCREEN_WIDTH
#define GAME_SCREEN_HEIGHT	LOWER_SCREEN_HEIGHT

#define DELTA_TIME			(1.f / 30.f)

#define PI					3.141592654f
#define RADTODEG			(180.f/PI)
#define DEGTORAD			(PI/180.f)

/***************************************************
*	Global Variables
***************************************************/

//Look-up table used for the fast trigometric functions
static const float sinTable[91] =
{
	0.000000f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f, 0.139173f, 0.156434f,
	0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f, 0.275637f, 0.292372f, 0.309017f, 0.325568f,
	0.342020f, 0.358368f, 0.374607f, 0.390731f, 0.406737f, 0.422618f, 0.438371f, 0.453990f, 0.469472f, 0.484810f,
	0.500000f, 0.515038f, 0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.629320f,
	0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.719340f, 0.731354f, 0.743145f, 0.754710f,
	0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 0.819152f, 0.829038f, 0.838671f, 0.848048f, 0.857167f,
	0.866025f, 0.874620f, 0.882948f, 0.891007f, 0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.933580f,
	0.939693f, 0.945519f, 0.951057f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.974370f, 0.978148f, 0.981627f,
	0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998630f, 0.999391f, 0.999848f,
	1.000000f
};

/***************************************************
*	Game namespace
***************************************************/

namespace Game
{
	/*
	*	Texture Info Struct
	*		Stores basic texture and sprite animation information
	*/
	typedef struct
	{
		int		id;						// ID of the texture image
		int		width;					// Width of the entire image
		int		height;					// Height of the entire image
		int		offsetX;				// X-value of top left point of first frame
		int		offsetY;				// Y-value of top left point of first frame
		int		maxFrame;				// Maximum frame of sprite animation
		int		framePerRow;			// Frame per row of sprite sheet
		int		framePerCol;			// Frame per column of sprite sheet
		int		widthPerFrame;			// Width of one frame
		int		heightPerFrame;			// Height of one frame

		//Sprite animation information
		float	currentFrame;			// Current frame of sprite animation - Stored as float so that it can be incremented in small amounts. Cast back to int when using
		float	animationSpeed;			// Speed modifier of the sprite animation
		int		animationState;			// State of the sprite animation. 0 = stopped, -1 = loop, +ve int = repeat x times
	} TextureInfo;

	/*
	*	Collision Shape Enumerator
	*		Type of collision check to use for the object
	*/
	enum ECOLLISIONSHAPE
	{
		ECOLLISIONSHAPE_CIRCLE,
		ECOLLISIONSHAPE_AABB,
		ECOLLISIONSHAPE_OBB,
		ECOLLISIONSHAPE_TRIANGLE,
		ECOLLISIONSHAPE_POINT,
		ECOLLISIONSHAPE_NONE,
		ECOLLISIONSHAPE_COUNT
	};

	/*
	*	2D Game Object Class
	*		Stores basic information such as position, velocity, direction and size
	*		Built-in functions to handle texture information and sprite animation
	*/
	class GameObject
	{
		protected:
			int					m_ObjectID;			// ID of the object that can be used for various purposes, e.g. type ID or count ID

			bool				m_Active;			// Basic boolean state that can be used for various purposes, e.g. alive state, alert state, etc.

			TextureInfo			m_TexInfo;			// Information of the texture set to the object, such as texture id, width and height
			float				m_Alpha;			// Alpha value of the texture from 0.f to 1.f

			bool				m_FlipHorizontal;	// Boolean of whether to flip the texture horizontally and vertically
			bool				m_FlipVertical;		// -Axis of flip is always with respect to the direction vector

			float				m_CollisionRadius;	// Radius of the circle collision checking
			ECOLLISIONSHAPE		m_CollisionShape;	// Type of shape to use for collision checking
		public:
			VECTOR2				m_Position;			// Centre position of the object
			VECTOR2				m_Velocity;			// Velocity of the object
			VECTOR2				m_Direction;		// Direction of the object. Can be set to be a different direction from velocity. Remember to always normalize this vector!
			VECTOR2				m_Size;				// Size of the object. Used for AABB and OBB collision checking
			VECTOR2				m_Scale;			// Scale of the object. Scales the texture but not the collision box.
			LinkedList<VECTOR2> m_Vertex;			// Vertexes of the object to use for more complex shape collisions (e.g. triangles or other polygons)

			// コンストラクタ
			GameObject();
			// デストラクタ
			~GameObject();

			// Get the x-value of the left end
			float GetLeft(void) {return m_Position.x - m_Size.x*0.5f;}
			// Get the x-value of the right end
			float GetRight(void) {return m_Position.x + m_Size.x*0.5f;}
			// Get the y-value of the top end
			float GetTop(void) {return m_Position.y - m_Size.y*0.5f;}
			// Get the y-value of the bottom end
			float GetBottom(void) {return m_Position.y + m_Size.y*0.5f;}
			// Get the object ID
			int GetObjectID(void) {return m_ObjectID;}
			// Get the active state
			bool IsActive(void) {return m_Active;}
			// Get the texture info
			TextureInfo GetTexture(void) {return m_TexInfo;}
			// Get the alpha value
			float GetAlpha(void) {return m_Alpha;}
			// Get the collision radius
			float GetCollisionRadius(void) {return m_CollisionRadius;}
			// Get the collision shape type
			ECOLLISIONSHAPE GetCollisionShape(void) {return m_CollisionShape;}
			// Get the size of the bounding box
			VECTOR2 GetBoundingSize(void);

			// Set the object ID
			void SetObjectID(int id) {m_ObjectID = id;}
			// Set the active state
			void SetActive(bool active) {m_Active = active;}
			// Set the texture information
			void SetTexture(int id, int widthPerFrame = 0, int heightPerFrame = 0, int offsetX = 0, int offsetY = 0, int maxFrame = 1);
			// Set the current animation frame
			void SetCurrentFrame(int frame)	{m_TexInfo.currentFrame = (float)frame;}
			// Set the alpha value
			void SetAlpha(float alpha) {m_Alpha = alpha;}
			// Set the horizontal flip state
			void SetFlipHorizontal(bool flip) {m_FlipHorizontal = flip;}
			// Set the vertical flip state
			void SetFlipVertical(bool flip) {m_FlipVertical = flip;}
			// Set the collision radius
			void SetCollisionRadius(float collisionRadius) {m_CollisionRadius = collisionRadius;}
			// Set the collision shape type
			void SetCollisionShape (ECOLLISIONSHAPE collisionShape) {m_CollisionShape = collisionShape;}

			// Calculate a collision radius automatically based on the size values.
			// Inner circle determines whether to calculate a collision circle that is within the square, or one that wraps around the square
			// If the size is not a square, diffSizeCalculationMethod determines whether to use the smaller, average or larger value of the two for calculation
			// -1 = use the smaller value, 0 = use the average value, 1 = use the larger value
			void CalcCollisionRadius(bool innerCircle = true, int diffSizeCalculationMethod = 0);

			// Plays a sprite animation with the specified speed and number of times (-1 to loop)
			// If relative is set to true, the animation will not be reset and animation state will be compounded to the previous state
			void PlayAnimation(float animationSpeed = 1.f, int animationState = 1, bool relative = false);
			// Stops the sprite animation. Resets both the animation state and speed
			void StopAnimation();

			// Checks for collision based off the collision shape of the object(s)
			bool CollideObject (GameObject* obj);
			bool CollideCircle (VECTOR2 circ_pos, float circ_radius);
			bool CollideAABB (VECTOR2 rect_pos, VECTOR2 rect_size);
			bool CollideOBB (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir);
			bool CollideTriangle (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3);
			bool CollidePoint (VECTOR2 point_pos);
			bool CollideLine (VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);

			// Gets the closest point on the current object from another point
			VECTOR2 GetClosestPoint (VECTOR2 point, bool clampToEdge = true);

			// 更新
			void Update(float deltaTime);

			// 描画
			virtual void Render(float offsetX = 0.f, float offsetY = 0.f, bool SwapScreen = false);
	};


	/*
	*	Collision functions
	*		Collision check functions between circle, AABB, OBB, triangle, point and line
	*		Line-collision functions can also optionally return the point and normal of collision
	*/

	// Collision with circle
	bool CollideCircleCircle (VECTOR2 circ1_pos, float circ1_radius, VECTOR2 circ2_pos, float circ2_radius);
	bool CollideAABBCircle (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 circ_pos, float circ_radius);
	bool CollideOBBCircle (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 circ_pos, float circ_radius);
	bool CollideTriangleCircle (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 circ_pos, float circ_radius);
	bool CollidePointCircle (VECTOR2 point_pos, VECTOR2 circ_pos, float circ_radius);
	bool CollideLineCircle (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 circ_pos, float circ_radius, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);

	// Collision with AABB
	bool CollideCircleAABB (VECTOR2 circ_pos, float circ_radius, VECTOR2 rect_pos, VECTOR2 rect_size);
	bool CollideAABBAABB (VECTOR2 rect1_pos, VECTOR2 rect1_size, VECTOR2 rect2_pos, VECTOR2 rect2_size);
	bool CollidePointAABB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size);
	bool CollideLineAABB (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);

	// Collision with OBB
	bool CollideCircleOBB (VECTOR2 circ_pos, float circ_radius, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir);
	bool CollideOBBOBB (VECTOR2 rect1_pos, VECTOR2 rect1_size, VECTOR2 rect1_dir, VECTOR2 rect2_pos, VECTOR2 rect2_size, VECTOR2 rect2_dir);
	bool CollidePointOBB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir);
	bool CollideLineOBB (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);

	// Collision with triangle
	bool CollideCircleTriangle (VECTOR2 circ_pos, float circ_radius, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3);
	bool CollidePointTriangle (VECTOR2 point_pos, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3);
	bool CollideLineTriangle (VECTOR2 line_start, VECTOR2 line_end, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);

	// Collision with point
	bool CollideCirclePoint (VECTOR2 circ_pos, float circ_radius, VECTOR2 point_pos);
	bool CollideAABBPoint (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 point_pos);
	bool CollideOBBPoint (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 point_pos);
	bool CollideTrianglePoint (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 point_pos);

	// Collision with line
	bool CollideCircleLine (VECTOR2 circ_pos, float circ_radius, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);
	bool CollideAABBLine (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);
	bool CollideOBBLine (VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);
	bool CollideTriangleLine (VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, VECTOR2 line_start, VECTOR2 line_end, VECTOR2* collisionPoint = NULL, VECTOR2* collisionNormal = NULL);
	bool CollideLineLine (VECTOR2 line1_start, VECTOR2 line1_end, VECTOR2 line2_start, VECTOR2 line2_end, VECTOR2* collisionPoint = NULL);


	/*
	*	2D vector functions
	*		Additional 2D vector functions
	*/

	// Get the (squared) length of the vector
	float Vec2LengthSq (VECTOR2 vec);
	float Vec2Length (VECTOR2 vec);

	// Get the angle of a vector relative to (0, -1)
	// +ve anti-clockwise and -ve clockwise
	float Vec2AngleRad (VECTOR2 vec);
	float Vec2AngleDeg (VECTOR2 vec);

	// Get the angle between 2 vectors
	float Vec2AngleRad (VECTOR2 vec1, VECTOR2 vec2);
	float Vec2AngleDeg (VECTOR2 vec1, VECTOR2 vec2);

	// Get the dot product of vec1 and vec2
	float Vec2Dot (VECTOR2 vec1, VECTOR2 vec2);

	// Get the perpendicular dot product - a modification of the dot product where 'a' is replaced with the perpendicular vector rotated to the left by 90 degrees
	float Vec2PerpDot (VECTOR2 vec1, VECTOR2 vec2);	

	// Get the unit vector of the vector
	VECTOR2 Vec2Normalize (VECTOR2 vec);

	// Reflect an incindence vector, vec1, about the reflection vector, vec2
	VECTOR2 Vec2Reflect (VECTOR2 vec1, VECTOR2 vec2);

	// Rotate a vector by a specified angle
	VECTOR2 Vec2RotateRad (VECTOR2 vec, float rad, bool fast_trigo = true);
	VECTOR2 Vec2RotateDeg (VECTOR2 vec, float deg, bool fast_trigo = true);

	// Get a vector perpendicular to the given vector in the clockwise or counter-clockwise direction
	VECTOR2 Vec2PerpCW (VECTOR2 vec);
	VECTOR2 Vec2PerpCCW (VECTOR2 vec);

	// Get the projection component/vector of vec1 on vec2
	float Vec2ProjComponent (VECTOR2 vec1, VECTOR2 vec2);
	VECTOR2 Vec2ProjVector (VECTOR2 vec1, VECTOR2 vec2);


	/*
	*	Miscellaneous functions
	*		Additional functions for various uses
	*/

	//
	bool EpsilonEqual(float a, float b);

	// Limit a number within the min and max value
	int clamp (int num, int min, int max);
	float clamp (float num, float min, float max);

	// Fast trigometric functions using a look-up table
	float fast_sin_rad (float rad);
	float fast_sin_deg (int deg);
	float fast_cos_rad (float rad);
	float fast_cos_deg (int deg);

	// Determine which side of the line is the point on
	float sign (VECTOR2 point, VECTOR2 line_start, VECTOR2 line_end);

	// Get the point on a geometry that is nearest to a specified point
	VECTOR2 GetClosestPointOnCircle (VECTOR2 point_pos, VECTOR2 circ_pos, float circ_radius, bool clampToEdge = true);
	VECTOR2 GetClosestPointOnAABB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, bool clampToEdge = true);
	VECTOR2 GetClosestPointOnOBB (VECTOR2 point_pos, VECTOR2 rect_pos, VECTOR2 rect_size, VECTOR2 rect_dir, bool clampToEdge = true);
	VECTOR2 GetClosestPointOnTriangle (VECTOR2 point_pos, VECTOR2 tri_p1, VECTOR2 tri_p2, VECTOR2 tri_p3, bool clampToEdge = true);
	VECTOR2 GetClosestPointOnLine (VECTOR2 point_pos, VECTOR2 line_start, VECTOR2 line_end, bool clampToLine = true);
}