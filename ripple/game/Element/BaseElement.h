/**
*	@file	BaseElement.h
*
*	@brief	All GameElement is inheritance to this class
*
*	@date	2013/10/25
*
*	@author	Kenta Minamitsu
*/
#pragma once
#include "../../NW_Lib/Utility/NW_Lib_utility.h"
#include "../../NW_Lib/Utility/NW_Lib_Constant.h"

namespace Game
{
	class BaseEelement
	{
	private:

		// Element position
		VECTOR3 m_pos;

		// Element on/off
		bool enable;

		// Element visible
		bool visible;

		// Element size(1.0=nomal)
		float size;

	public:
		
		/**
			Constructor 
		*/ 
		BaseEelement();

		/**
			virtual Destructor 
		*/ 
		virtual ~BaseEelement();
		
		/**
			virtual Initialize
		*/ 
		virtual void Initialize();
		
		/**
			virtual Update
		*/ 
		virtual void Update();
		
		/**
			virtual DrawUpper
		*/ 
		virtual void DrawUpper();
		
		/**
			virtual DrawLower
		*/ 
		virtual void DrawLower();
		
	};

}// Game