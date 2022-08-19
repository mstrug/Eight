/*
 ============================================================================
 Name		: EightWindowTileSettings.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowTileSettings declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWTILESETTINGS_H
#define EIGHTWINDOWTILESETTINGS_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"

// CLASS DECLARATION

const TInt KEightWindowTileSettingsId = 0x1020;

/**
 *  CEightWindowTileSettings
 * 
 */
class CEightWindowTileSettings : public CEightFormWindow, MEightFormWindowObserver
    {
public:

    ~CEightWindowTileSettings();

    static CEightWindowTileSettings* NewL( MEightFormWindowObserver* aObserver, TSize aSize, CEightTile& aTile );
    
    void Activated();
    
    void Deactivated();
    
private:

    CEightWindowTileSettings( MEightFormWindowObserver* aObserver, TSize aSize, CEightTile& aTile );

    void ConstructL();
    
    void UpdateButtonColor();

    void WindowClosedByGesture();
    
protected: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );

protected: // from MEightFormWindowObserver
    
    void FormWindowButtonAction( TUint aWindowId, TUint aButtonId );
    
private:
    
    CEightTile& iTile;
    
    };

#endif // EIGHTWINDOWTILESETTINGS_H
