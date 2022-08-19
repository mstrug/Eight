/*
 ============================================================================
 Name		: EightWindowSetup.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightWindowSetup declaration
 ============================================================================
 */

#ifndef EIGHTWINDOWTILESETUP_H
#define EIGHTWINDOWTILESETUP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "EightFormWindow.h"
#include "EightTile.h"

// CLASS DECLARATION

const TInt KEightWindowSetupId = 0x1025;
const TInt KEightWindowSetupResetButtonId = 20;
const TInt KEightWindowSetupMenuButtonId = 10;

/**
 *  CEightWindowSetup
 * 
 */
class CEightWindowSetup : public CEightFormWindow, MEightFormWindowObserver, MEightMsgBoxObserver
    {
public:

    ~CEightWindowSetup();

    static CEightWindowSetup* NewL( /*MEightFormWindowObserver* aObserver, TUint aWindowId, */ TSize aSize );

    void Activated();

    void Deactivated();
    
private:

    CEightWindowSetup( /*MEightFormWindowObserver* aObserver, TUint aWindowId,*/ TSize aSize );

    void ConstructL();
    
    void UpdateButtonsColors();

    void UpdateButtonStates();

    void UpdateSettings();

    void WindowClosedByGesture();
    
protected: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );

protected: // from MEightFormWindowObserver
    
    void FormWindowButtonAction( TUint aWindowId, TUint aButtonId );
    
protected: // from MEightMsgBoxObserver
    
    void MsgBoxAction( TUint aMessageId, TInt aActionId );
    
private:
        
    };

#endif // EIGHTWINDOWTILESETUP_H
