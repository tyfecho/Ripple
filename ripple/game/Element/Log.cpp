/**
*	@file	Log.h
*
*	@brief	GameElement "Log" class
*
*	@date	2013/10/25
*
*	@author	Kenta Minamitsu
*/
#include "Log.h"

namespace Game
{
	
		/*
			Constructor 
		*/ 
		Log::Log()
			:BaseEelement()
		{

		}

		/*
			Load from file
		*/ 
		BaseEelement* Log::Create()
		{
			Log* obj = new Log();

			//Initializing
			
			obj->BaseEelement::Initialize();
			obj->Initialize();
			return obj;
		}

		/*
			Destructor 
		*/ 
		Log::~Log()
		{

		}
		
		/*
			Initialize
		*/ 
		void Log::Initialize()
		{

		}
		
		/*
			Update
		*/ 
		void Log::Update()
		{

		}
		
		/*
			DrawUpper
		*/ 
		void Log::DrawUpper()
		{

		}
		
		/*
			DrawLower
		*/ 
		void Log::DrawLower()
		{

		}
		
}// Game