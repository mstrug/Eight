/*
 ============================================================================
 Name		: TouchButtonOnOff.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonOnOff declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONONOFF_H
#define TOUCHBUTTONONOFF_H

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
 *  CTouchButtonOnOff
 * 
 */
class CTouchButtonOnOff : public CTouchControlBase
	{
public:

	~CTouchButtonOnOff();

	static CTouchButtonOnOff* NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager );

    void SetValue( TBool aOn );
    
    TBool IsOn() const;

    void SetRect( const TVgRect& aRect );

    void SetTextOffset( TInt aOffsetY );
    
    void UpdateColorsFromTheme();
    
    void UpdateColorsFromIndex( TInt aColorIndex );

    void SetActiveColor( TVgColor aColor );
    
	void Draw();

	void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
	
private:

	CTouchButtonOnOff( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, TUint aFontId, CVgFontManager& aFontManager );

	void ConstructL( const TDesC& aText );

	void UpdateGraphics();
	
private: // data
	
    VGPath iPath;

    VGPath iPathColor;

    VGPath iPathPressed;
    
    VGPaint iPaint;

    VGPaint iPaintColor;
    
	RBuf iText;
	
	TBool iOn;
	
	TUint iFontId;
	
	CVgFontManager& iFontManager;
	
	TInt iOffsetY;
	
	TVgRect iSwitchRect;
	
	};

#endif // TOUCHBUTTONONOFF_H
