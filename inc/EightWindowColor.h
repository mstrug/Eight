/*
 ============================================================================
 Name		: EightWindowColor.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowColor declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWTILECOLOR_H
#define EIGHTWINDOWTILECOLOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"

// CLASS DECLARATION

const TInt KEightWindowColorId = 0x1026;

/**
 *  CEightWindowColor
 * 
 */
class CEightWindowColor : public CEightFormWindow
    {
public:

    ~CEightWindowColor();

    static CEightWindowColor* NewL( MEightFormWindowObserver* aObserver, TSize aSize, TBool aUpdatePalette = ETrue, TBool aButtonNext = EFalse );
    
    void SelectColor( TVgColor aColor );

private:

    CEightWindowColor( MEightFormWindowObserver* aObserver, TSize aSize, TBool aUpdatePalette, TBool aButtonNext );

    void ConstructL();

    void WindowClosedByGesture();
    
protected: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );
    
private:
    
    TBool iUpdatePalette;
    
    TBool iButtonNext;
    
    TInt iDefaultColorBtnId;
    
    };

#endif // EIGHTWINDOWTILECOLOR_H
