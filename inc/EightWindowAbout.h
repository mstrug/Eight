/*
 ============================================================================
 Name		: EightWindowAbout.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowAbout declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWTILEABOUT_H
#define EIGHTWINDOWTILEABOUT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"

// CLASS DECLARATION

const TInt KEightWindowAboutId = 0x1023;

/**
 *  CEightWindowAbout
 * 
 */
class CEightWindowAbout : public CEightFormWindow
    {
public:

    ~CEightWindowAbout();

    static CEightWindowAbout* NewL( /*MEightFormWindowObserver* aObserver, TUint aWindowId, */ TSize aSize );

    void Draw();
    
private:

    CEightWindowAbout( /*MEightFormWindowObserver* aObserver, TUint aWindowId,*/ TSize aSize );

    void ConstructL();

    void WindowClosedByGesture();
    
    void TouchButtonPressed( TUint aId );

private:
    
    };

#endif // EIGHTWINDOWTILEABOUT_H
