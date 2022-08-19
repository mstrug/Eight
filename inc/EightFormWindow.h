/*
 ============================================================================
 Name		: EightFormWindow.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightFormWindow declaration
 ============================================================================
 */

#ifndef EIGHTFORMWINDOW_H
#define EIGHTFORMWINDOW_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <mw/akntouchgesturefwevents.h>
#include "TouchControlBase.h"
#include "TouchButtonObserver.h"
#include "EightMsgBox.h"


// CLASS DECLARATION
class CEightFormWindowManager;

class MEightFormWindowObserver
    {
public:
    virtual void FormWindowButtonAction( TUint aWindowId, TUint aButtonId ) = 0;
    };

/**
 *  CEightFormWindow
 * 
 */
class CEightFormWindow : public CBase, public MTouchButtonObserver
    {
public:

    ~CEightFormWindow();

    static CEightFormWindow* NewL( MEightFormWindowObserver* aObserver, TUint aWindowId, TSize aSize, const TDesC& aTitle );
    
    void AddButtonL( CTouchControlBase* aButton );

    CTouchControlBase* GetButton( TUint aButtonId );
    
    TUint Id() const;

    virtual void Activated();

    virtual void Deactivated();
    
    void SetObserver( MEightFormWindowObserver* aObserver );
    
    void SetWindowManager( CEightFormWindowManager* aManager ); // set automatically after showing of the window
    
    void SetMsgBox( CEightMsgBox* aMsgBox );
    
    TBool IsShowinOrHiding();
    
    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

    void HandleTouchGestureL( AknTouchGestureFw::MAknTouchGestureFwEvent& aEvent );
    
    virtual void Draw();
    
    virtual void Update( TTimeIntervalMicroSeconds32 aTimeInterval );
    
protected:

    CEightFormWindow( MEightFormWindowObserver* aObserver, TUint aWindowId, TSize aSize );

    void ConstructL( const TDesC& aTitle );

    TInt WindowContentHeight();
    
    virtual void WindowClosedByGesture();
    
protected: // from MTouchButtonObserver
    
    void TouchButtonPressed( TUint aId );

protected:

    MEightFormWindowObserver* iObserver;
    
    CEightFormWindowManager* iWindowManager; // not owned
    
    CEightMsgBox* iMsgBox; // not owned
    
    TUint iWindowId;
    
    TSize iSize;
    
    VGPath iBackgroundPath;

    VGPaint iBackgroundPaint;

    RPointerArray< CTouchControlBase > iButtons;
    
    RBuf iTitle;

    VGfloat iOffsetY;
    
    TInt iTitleOffset;
    
    };

#endif // EIGHTFORMWINDOW_H
