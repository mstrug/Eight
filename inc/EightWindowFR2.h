/*
 ============================================================================
 Name		: EightWindowFR2.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowFR2 declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWFR2_H
#define EIGHTWINDOWFR2_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"
#include "EightTileUiStatic.h"

// CLASS DECLARATION

const TInt KEightWindowFR2Id = 0x2002;

/**
 *  CEightWindowFR2
 * 
 */
class CEightWindowFR2 : public CEightFormWindow
	{
public:

	~CEightWindowFR2();

	static CEightWindowFR2* NewL( TSize aSize );

    void Activated();

    void Draw();
    
private:

	CEightWindowFR2( TSize aSize );

	void ConstructL();

	void TouchButtonPressed( TUint aId );

    void WindowClosedByGesture();

private: // data
    
    CEightTile* iTile;

    CEightTile* iTile2;
    
    CEightTileUiStatic* iIcons;
    
	};

#endif // EIGHTWINDOWFR2_H
