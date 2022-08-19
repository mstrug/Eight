/*
 ============================================================================
 Name		: TouchButtonText.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuItemBase declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONTEXT_H
#define TOUCHBUTTONTEXT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h> 
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgFontManager.h>
#include <SGEVg/VgFont.h>
#include "TouchControlBase.h"


// CLASS DECLARATION
class MTouchButtonObserver;

const TInt KTextOffsetMedium = 8;

/**
 *  CTouchButtonText
 * 
 */
class CTouchButtonText : public CTouchControlBase
	{
    
public:
	
	~CTouchButtonText();

    static CTouchButtonText* NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aTextRect, TVgRect aButtonRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager );

    void SetRect( const TVgRect& aRect );
    
    void SetBackgroundColor( TVgColor aColor );

    void SetSelectedBackgroundColor( TVgColor aColor );

    void SetTextColor( TVgColor aColor );
    
    void SetBorderWidth( VGfloat aWidth );

    void SetLinkStyle( TVgRect aRect, TVgColor aColor );
    
	void Draw();

	void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
	
protected:

	CTouchButtonText( MTouchButtonObserver* aObserver, TUint aId, TVgRect aTextRect, TVgRect aButtonRect, TUint aFontId, CVgFontManager& aFontManager );

	void ConstructL( const TDesC& aText );

protected:
	
	CVgFontManager& iFontManager;
	
	VGPath iPath;

    VGPaint iPaint;

    VGPaint iBackgroundPaint;

    VGPaint iTextPaint;
	
    VGPaint iPressedTextPaint;
	
    TVgRect iTextRect;
	
    TUint iFontId;
    
	RBuf iText;
	
	VGfloat iBorderWidth;
	
	TBool iLinkStyle;

    VGPath iUnderlinedPath;
    
	};


#endif // TOUCHBUTTONPATH_H
