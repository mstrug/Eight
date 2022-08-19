/*
 ============================================================================
 Name		: TouchButtonPath.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuItemBase declaration
 ============================================================================
 */

#ifndef TOUCHBUTTONPATH_H
#define TOUCHBUTTONPATH_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h> 
#include <SGEVg/VgPrimitives.h>
#include <SGEVg/VgSvgImage.h>
#include <SGEVg/VgFont.h>
#include "TouchControlBase.h"


// CLASS DECLARATION
class MTouchButtonObserver;

/**
 *  CTouchButtonPath
 * 
 */
class CTouchButtonPath : public CTouchControlBase
	{
    
public:
	
	~CTouchButtonPath();

    static CTouchButtonPath* NewL( MTouchButtonObserver* aObserver, TUint aId, TVgPoint aImagePosition, TVgRect aButtonRect, CVgSvgImage* aImage, CVgSvgImage* aPressedImage );
	
	TVgRect RectWithOffset( TBool aGrowed = EFalse ) const;
    
    void SetImagePosition( const TVgPoint& aPosition );

	TVgPoint ButtonOffset() const;
	
	void SetImageAngle( TReal aAngle );
	
	void SetButtonRectOffset( TVgPoint aOffset );

	void DrawDebug();
	
	void Draw();

	void Update( TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );

protected:

	CTouchButtonPath( MTouchButtonObserver* aObserver, TUint aId, TVgPoint aImagePosition, TVgRect aButtonRect, CVgSvgImage* aImage, CVgSvgImage* aPressedImage );

	void ConstructL();

protected:
    
	CVgSvgImage* iImage; // not owned
    
	CVgSvgImage* iPressedImage; // not owned
	
	TVgPoint iImagePosition;
	
	TVgPoint iButtonOffset;
	
	TReal iImageAngle;
	};


#endif // TOUCHBUTTONPATH_H
