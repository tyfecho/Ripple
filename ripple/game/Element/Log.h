/**
*	@file	Log.h
*
*	@brief	GameElement "Log" class
*
*	@date	2013/10/25
*
*	@author	Kenta Minamitsu
*/
#pragma once
#include "BaseElement.h"

namespace Game
{
	class Log : public BaseEelement
	{
	private:

		// log rotation. span to deg 45. 
		int m_degRot;

	public:
		
		/**
			Constructor 
		*/ 
		Log();

		/**
			Load from file
		*/ 
		static BaseEelement* Create();

		/**
			Destructor 
		*/ 
		~Log();
		
		/**
			Initialize
		*/ 
		void Initialize();
		
		/**
			Update
		*/ 
		void Update();
		
		/**
			DrawUpper
		*/ 
		void DrawUpper();
		
		/**
			DrawLower
		*/ 
		void DrawLower();

	};

}// Game