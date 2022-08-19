/*
 ============================================================================
 Name		: TouchButtonBase.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonBase declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONBASE_H
#define TOUCHBUTTONBASE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h> 
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>

// CLASS DECLARATION
class MTouchButtonObserver;

/**
 *  CTouchButtonBase
 * 
 */
class CTouchButtonBase : public CBase
    {
public:

    ~CTouchButtonBase();
    
    TUint Id() const;
    
    TVgRect Rect() const;
    
    virtual void SetRect( const TVgRect& aRect );

    void SetObserver( MTouchButtonObserver* aObserver );
    
    TBool IsPressed() const;
    
    void Unpress();
    
    void SetVisible( TBool aVisible );

    void SetEnabled( TBool aEnable );

    virtual void UpdateColorsFromTheme();
    
    virtual void UpdateColorsFromIndex( TInt aColorIndex );
    
    virtual void Draw();

    virtual void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    // returns ETrue when consumed
    virtual TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

protected:

    CTouchButtonBase( MTouchButtonObserver* aObserver, TUint aId, TVgRect aRect );

    void DrawDisabledRect();
    
protected:
    
    MTouchButtonObserver* iObserver;
    
	VGPath iDisabledPath;

    VGPaint iDisabledPaint;
    
    TUint iId;
    
    TVgRect iRect;
    
    TBool iPressed;
    
    TBool iVisible;

    TBool iEnable;
    
    TPoint iP;
    
    };

#endif // TOUCHBUTTONBASE_H
