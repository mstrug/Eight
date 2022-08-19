/*
 ============================================================================
 Name		: EightWindowPremium.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowPremium declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWPREMIUM_H
#define EIGHTWINDOWPREMIUM_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"

// CLASS DECLARATION

const TInt KEightWindowPremiumId = 0x1027;

/**
 *  CEightWindowPremium
 * 
 */
class CEightWindowPremium : public CEightFormWindow
	{
public:

	~CEightWindowPremium();

	static CEightWindowPremium* NewL( TSize aSize );

private:

	CEightWindowPremium( TSize aSize );

	void ConstructL();

    void WindowClosedByGesture();

	};

#endif // EIGHTWINDOWPREMIUM_H
