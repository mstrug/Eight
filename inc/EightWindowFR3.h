/*
 ============================================================================
 Name		: EightWindowFR3.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR3 declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWFR3_H
#define EIGHTWINDOWFR3_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"
#include "EightCompilationSettings.h"

// CLASS DECLARATION

const TInt KEightWindowFR3Id = 0x2003;

/**
 *  CEightWindowFR3
 * 
 */
class CEightWindowFR3 : public CEightFormWindow
	{
public:

	~CEightWindowFR3();

	static CEightWindowFR3* NewL( TSize aSize );

	void Activated();
	
	void Draw();

private:

	CEightWindowFR3( TSize aSize );

	void ConstructL();

	void TouchButtonPressed( TUint aId );

    void WindowClosedByGesture();

private: // data
    
    TFixedArray< CEightTile*, 
#ifdef EIGHT_PREMIUM
    4
#else
    5
#endif
    > iTiles;

	};

#endif // EIGHTWINDOWFR3_H
