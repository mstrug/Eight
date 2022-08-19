/*
 ============================================================================
 Name		: EightWindowFR1.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR1 declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWFR1_H
#define EIGHTWINDOWFR1_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"

// CLASS DECLARATION

const TInt KEightWindowFR1Id = 0x2001;

/**
 *  CEightWindowFR1
 * 
 */
class CEightWindowFR1 : public CEightFormWindow
	{
public:

	~CEightWindowFR1();

	static CEightWindowFR1* NewL( TSize aSize );

private:

	CEightWindowFR1( TSize aSize );

	void ConstructL();

	void TouchButtonPressed( TUint aId );

    void WindowClosedByGesture();

	};

#endif // EIGHTWINDOWFR1_H
