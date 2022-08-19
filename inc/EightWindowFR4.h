/*
 ============================================================================
 Name		: EightWindowFR4.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR4 declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWFR4_H
#define EIGHTWINDOWFR4_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"

// CLASS DECLARATION

const TInt KEightWindowFR4Id = 0x2004;

/**
 *  CEightWindowFR4
 * 
 */
class CEightWindowFR4 : public CEightFormWindow
	{
public:

	~CEightWindowFR4();

	static CEightWindowFR4* NewL( TSize aSize );

private:

	CEightWindowFR4( TSize aSize );

	void ConstructL();

	void TouchButtonPressed( TUint aId );

    void WindowClosedByGesture();

	};


#endif // EIGHTWINDOWFR4_H
