/*
 ============================================================================
 Name		: EightFormWindowManager.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightFormWindowManager declaration
 ============================================================================
 */

#ifndef EIGHTFORMWINDOWMANAGER_H
#define EIGHTFORMWINDOWMANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <mw/akntouchgesturefwevents.h>
#include "EightFormWindow.h"


// CLASS DECLARATION

/**
 *  CEightFormWindowManager
 * 
 */
class CEightFormWindowManager : public CBase
    {
public:

    ~CEightFormWindowManager();

    static CEightFormWindowManager* NewL();

    void ShowWindowL( CEightFormWindow* aWindow, TBool aTakeWindowOwnership = ETrue );

    void HideWindow( TUint aWindowId );

    TBool CloseWindow( TUint aWindowId ); // removes window

    void CloseAllWindows(); // removes window

    void SetNextWindowActivateOnClose();
    
    TBool IsVisible() const;
    
    TBool IsShowinOrHidingWindow();

    TInt WindowsCount() const;

    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

    void HandleTouchGestureL( AknTouchGestureFw::MAknTouchGestureFwEvent& aEvent );
    
    void Draw();
    
    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

private:

    CEightFormWindowManager();

    void ConstructL();

    void CloseWindowInternal();

private:
    
    TBool iVisible;
    
    class CItem : public CBase
        {
    public:
        CEightFormWindow* iWindow;
        TBool iOwned;
        ~CItem() { if ( iOwned ) delete iWindow; else iWindow->SetWindowManager( NULL ); };
        };
    RPointerArray< CItem > iWindows;
    
    TInt iWindowIdxToClose;
    
    TBool iNextWindowActivateOnClose;
    
    TBool iClosingAllWindows;
    
    };

#endif // EIGHTFORMWINDOWMANAGER_H
