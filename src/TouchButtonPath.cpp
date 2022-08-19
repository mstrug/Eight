/*
 ============================================================================
 Name		: TouchButtonPath.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuItemBase implementation
 ============================================================================
 */

#include "TouchButtonPath.h"
#include "EightSettings.h"
#include "TouchButtonObserver.h"
#include "vg/vgu.h"



CTouchButtonPath::CTouchButtonPath( MTouchButtonObserver* aObserver, TUint aId, TVgPoint aImagePosition, TVgRect aButtonRect, CVgSvgImage* aImage, CVgSvgImage* aPressedImage ) 
    : CTouchControlBase( aObserver, aId, aButtonRect ), iImage( aImage ), iPressedImage( aPressedImage ), iImagePosition( aImagePosition )
	{
	}

CTouchButtonPath::~CTouchButtonPath()
	{
	}

CTouchButtonPath* CTouchButtonPath::NewL( MTouchButtonObserver* aObserver, TUint aId, TVgPoint aImagePosition, TVgRect aButtonRect, CVgSvgImage* aImage, CVgSvgImage* aPressedImage )
    {
    CTouchButtonPath* self = new (ELeave) CTouchButtonPath( aObserver, aId, aImagePosition, aButtonRect, aImage, aPressedImage );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self;
    return self;
    }

void CTouchButtonPath::ConstructL()
	{
	}

void CTouchButtonPath::SetImagePosition( const TVgPoint& aPosition )
    {
    iImagePosition = aPosition;
    }


TVgRect CTouchButtonPath::RectWithOffset( TBool aGrowed ) const
	{
	TVgRect r( iRect );
    r.Move( iButtonOffset );
    if ( aGrowed )
    	{
    	r.Grow( 10, 10 );
    	}
    return r;
	}

TVgPoint CTouchButtonPath::ButtonOffset() const
    {
    return iButtonOffset;
    }

void CTouchButtonPath::SetImageAngle( TReal aAngle )
    {
    iImageAngle = aAngle;
    }

void CTouchButtonPath::SetButtonRectOffset( TVgPoint aOffset )
    {
    iButtonOffset = aOffset;
    }

void CTouchButtonPath::DrawDebug()
    {
    VGPath path1 = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    TVgColor cl( 0xff0000a0 );
    VGPaint paint = cl.CreateVgPaint();
    TVgRect r( iRect );
    r.Move( iButtonOffset );

    vgSetPaint( paint, VG_FILL_PATH );
    vguRect( path1, r.iTl.iX, r.iBr.iY, r.Width(), r.Height() );
    vgDrawPath( path1, VG_FILL_PATH );
    vgClearPath( path1, VG_PATH_CAPABILITY_ALL );
    vgDestroyPaint( paint );

    TVgColor cl2( 0x00ff0040 );
    VGPaint paint2 = cl2.CreateVgPaint();
    r.Grow( 10, 10 );
    vgSetPaint( paint2, VG_FILL_PATH );
    vguRect( path1, r.iTl.iX, r.iBr.iY, r.Width(), r.Height() );
    vgDrawPath( path1, VG_FILL_PATH );
    vgClearPath( path1, VG_PATH_CAPABILITY_ALL );
    vgDestroyPaint( paint2 );    
    
    vgDestroyPath( path1 );

    CEightSettings* settings = CEightSettings::GetInstance();
    CVgFont* font = settings->FontManager().Get( 1 );
    if ( font )
    	{
    	TBuf<64> buf;
        TVgRect r( iRect );
        r.Move( iButtonOffset );
    	( iPressed ? buf.Append( _L("pressed") ) : buf.Append( _L("n") ) );
    	font->DrawText( r.iTl, buf );
    	
    	buf.Zero();
    	buf.Format( _L("%d,%d %d,%d"), (TInt)r.iTl.iX, (TInt)r.iTl.iY, (TInt)r.iBr.iX, (TInt)r.iBr.iY );
    	font->DrawText( TVgPoint( r.iTl.iX, r.iTl.iY + 16 ), buf );
    	
    	buf.Zero();
    	buf.Format( _L("%d,%d"), (TInt)iP.iX, (TInt)iP.iY );
    	font->DrawText( TVgPoint( r.iTl.iX, r.iTl.iY + 2 * 16 ), buf );
    	}
    }

void CTouchButtonPath::Draw()
	{
    if ( !iVisible )
        {
        return;
        }
    if ( iPressed )
        {
        iPressedImage->Draw( iImagePosition + iButtonOffset, iImageAngle );
        }
    else
        {
        iImage->Draw( iImagePosition + iButtonOffset, iImageAngle );
        }
    
    if ( !iEnable )
    	{
    	DrawDisabledRect();
    	}
	}

void CTouchButtonPath::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
	{
	}

TBool CTouchButtonPath::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{
    if ( !iVisible || !iEnable )
        {
        return EFalse;
        }
    TVgRect r( iRect );
    r.Move( iButtonOffset );
    if ( !iPressed && aPointerEvent.iType == TPointerEvent::EButton1Down && r.Contains( aPointerEvent.iPosition ) )
        {
        iPressed = ETrue;
        iP = aPointerEvent.iPosition;
        return ETrue;
        }
    else if ( iPressed && aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        iPressed = EFalse;
        iP = aPointerEvent.iPosition;
        r.Grow( 10, 10 );
        if ( r.Contains( aPointerEvent.iPosition ) && iObserver )
            {
            iObserver->TouchButtonPressed( iId );
            return ETrue;
            }
        return EFalse;
        }
    return EFalse;
	}

