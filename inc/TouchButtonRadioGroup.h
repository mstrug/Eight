/*
 ============================================================================
 Name		: TouchButtonRadioGroup.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonRadioGroup declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONRADIOGROUP_H
#define TOUCHBUTTONRADIOGROUP_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h> 
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFontManager.h>
#include <SGEVg/VgFont.h>
#include "TouchControlBase.h"

// CLASS DECLARATION
class MTouchButtonObserver;

/**
 *  CTouchButtonRadioGroup
 * 
 */
class CTouchButtonRadioGroup : public CTouchControlBase
    {
public:

    ~CTouchButtonRadioGroup();

    static CTouchButtonRadioGroup* NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, const TDesC& aLabelText, TUint aLabelFontId, TUint aItemFontId, CVgFontManager& aFontManager );
    
    void AddButtonL( TUint aId, const TDesC& aButtonText );
    
    void SetSelectedButtonId( TUint aId );

    TInt SelectedButtonId() const;

    void SetRect( const TVgRect& aRect );
    
    void Draw();

    void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
private:

    CTouchButtonRadioGroup( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, TUint aLabelFontId, TUint aItemFontId, CVgFontManager& aFontManager );

    void ConstructL( const TDesC& aLabelText );

    void UpdateRects( TVgRect& aRect );
    
    void UpdateGraphic( TInt aIdx );

    void UnselectAllButtons();
    
private:

    VGPath iPath;
    
    VGPaint iPaint;
    VGPaint iPaintSel;
    
    RBuf iLabelText;
    
    TUint iLabelFontId;

    TUint iButtonFontId;
    
    CVgFontManager& iFontManager;
    
    struct SButton
        {
        VGPath iPath;
        TUint iId;
        RBuf iText;
        TVgRect iRect;
        TBool iSelected;
        TBool iPressed;
        };
    
    RArray< SButton > iButtons;
    
    };

#endif // TOUCHBUTTONRADIOGROUP_H
