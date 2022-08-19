/*
 ============================================================================
 Name		: TouchButtonOnOff.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonOnOff implementation
 ============================================================================
 */

#include "TouchButtonOnOff.h"
#include "TouchButtonObserver.h"
#include "EightSettings.h"


CTouchButtonOnOff::CTouchButtonOnOff( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, TUint aFontId, CVgFontManager& aFontManager )
	: CTouchControlBase( aObserver, aId, aButtonRect ), iFontId( aFontId ), iFontManager( aFontManager )
	{
	}

CTouchButtonOnOff::~CTouchButtonOnOff()
	{
	iText.Close();
	
    vgDestroyPath( iPath );
    vgDestroyPath( iPathColor );
    vgDestroyPath( iPathPressed );
    vgDestroyPaint( iPaint );
    vgDestroyPaint( iPaintColor );
	}

CTouchButtonOnOff* CTouchButtonOnOff::NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager )
	{
	CTouchButtonOnOff* self = new (ELeave) CTouchButtonOnOff( aObserver, aId, aButtonRect, aFontId, aFontManager );
	CleanupStack::PushL(self);
	self->ConstructL( aText );
	CleanupStack::Pop(); // self;
	return self;
	}

void CTouchButtonOnOff::ConstructL( const TDesC& aText )
	{
	iText.Create( aText );

	iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );

	iPathColor = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
	
	iPathPressed = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
	
    iPaint = CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.CreateVgPaint();
    
    iPaintColor = CEightSettings::GetInstance()->iPalette.iTileColor.CreateVgPaint();

    UpdateGraphics();
	}

void CTouchButtonOnOff::SetValue( TBool aOn )
	{
	iOn = aOn;
	UpdateGraphics();
	}

TBool CTouchButtonOnOff::IsOn() const
	{
	return iOn;
	}

void CTouchButtonOnOff::SetRect( const TVgRect& aRect )
	{
	CTouchControlBase::SetRect( aRect );
	UpdateGraphics();
	}

void CTouchButtonOnOff::SetTextOffset( TInt aOffsetY )
	{
	iOffsetY = aOffsetY;
	}

void CTouchButtonOnOff::UpdateColorsFromTheme()
	{
    vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.iColor.Begin() );
    vgSetParameterfv( iPaintColor, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iTileColor.iColor.Begin() );
	}

void CTouchButtonOnOff::UpdateColorsFromIndex( TInt aColorIndex )
	{
    vgSetParameterfv( iPaintColor, VG_PAINT_COLOR, 4, TVgColor( KEightColorsBase[ aColorIndex ] ).iColor.Begin() );
	}

void CTouchButtonOnOff::SetActiveColor( TVgColor aColor )
	{
    vgSetParameterfv( iPaintColor, VG_PAINT_COLOR, 4, aColor.iColor.Begin() );
	}

void CTouchButtonOnOff::UpdateGraphics()
	{
    vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
    vgClearPath( iPathColor, VG_PATH_CAPABILITY_ALL );
    vgClearPath( iPathPressed, VG_PATH_CAPABILITY_ALL );

	TInt w = 60;
	TInt h = 25;
    TVgRect r( TPoint( iRect.iBr.iX - w, iRect.iTl.iY - iRect.Height() / 2 + h / 2 ), TSize( w, h ) );
    
	TInt b = 2;
	TInt wb = 12;
    TInt wbm = 3;
    TInt wbm2 = 3;
	if ( iOn )
		{
		TVgRect rwbOn( TVgPoint( r.iBr.iX - wb, r.iTl.iY ), TVgSize( wb, h ) );
		TVgRect rOnFill( TVgPoint( r.iTl.iX + b + wbm, r.iTl.iY - b - wbm ), TVgSize( r.Width() - b - wbm - rwbOn.Width() - wbm2, r.Height() - 2 * b - 2 * wbm ) );
		TVgRect bOnTop( TVgPoint( r.iTl.iX + b, r.iTl.iY ), TVgSize( w - wb - wbm2 - b, b ) );
		TVgRect bOnBottom( TVgPoint( r.iTl.iX + b, r.iBr.iY + b ), bOnTop.Size() );
		TVgRect bOnLeft( r.iTl, TVgSize( b, h ) );
		
	    rwbOn.AddVguRect( iPath );
	    bOnTop.AddVguRect( iPath );
	    bOnBottom.AddVguRect( iPath );
	    bOnLeft.AddVguRect( iPath );
	    rOnFill.AddVguRect( iPathColor );
	    
	    //rOnFill.AddVguRect( iPathPressed );
		}
	else
		{
		TVgRect rwbOff( TVgPoint( r.iTl.iX, r.iTl.iY ), TVgSize( wb, h ) );
        TVgRect rOffFill( TVgPoint( r.iTl.iX + wb + wbm2, r.iTl.iY - b - wbm ), TVgSize( r.Width() - b - wbm - rwbOff.Width() - wbm2, r.Height() - 2 * b - 2 * wbm ) );
		TVgRect bOffTop( TVgPoint( r.iTl.iX + wb + wbm2, r.iTl.iY ), TVgSize( w - wb - wbm2 - b, b ) );
		TVgRect bOffBottom( TVgPoint( r.iTl.iX + wb + wbm2, r.iBr.iY + b ), bOffTop.Size() );
		TVgRect bOffRight( TVgPoint( r.iBr.iX - b, r.iTl.iY ), TVgSize( b, h ) );

	    rwbOff.AddVguRect( iPath );
	    bOffTop.AddVguRect( iPath );
	    bOffBottom.AddVguRect( iPath );
	    bOffRight.AddVguRect( iPath );

	    //rOffFill.AddVguRect( iPathPressed );
		}

	r.Shrink( 1, 1 );
	r.AddVguRect( iPathPressed );
	
	iSwitchRect = r;
	}

void CTouchButtonOnOff::Draw()
	{
	if ( !iVisible )
		{
		return;
		}

    if ( iPressed )
        {
        vgSetPaint( iPaint, VG_STROKE_PATH );
        vgDrawPath( iPathPressed, VG_STROKE_PATH );
        }
    else
        {
        if ( iOn )
            {
            vgSetPaint( iPaintColor, VG_FILL_PATH );
            vgDrawPath( iPathColor, VG_FILL_PATH );
            }
        
        vgSetPaint( iPaint, VG_FILL_PATH );
        vgDrawPath( iPath, VG_FILL_PATH );
        }

    CVgFont* font = iFontManager.Get( iFontId );
    if ( font )
        {
//        if ( iPressed )
//            {
//            vgSetPaint( iPaint, VG_FILL_PATH );
//            vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_MULTIPLY );
//            font->DrawText( iRect, iText, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalCenter );
//            vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL );
//            }
//        else
            {
            TVgRect r( iRect );
            r.iBr.iY += iOffsetY;
            font->DrawText( r, iText, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalCenter  );
            }
        }
    
    if ( !iEnable )
    	{
    	DrawDisabledRect();
    	}
	}

void CTouchButtonOnOff::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
	{
	}

TBool CTouchButtonOnOff::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{
    if ( !iVisible || !iEnable )
        {
        return EFalse;
        }
    TVgRect r( iSwitchRect );
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
        	iOn = !iOn;
        	UpdateGraphics();
            iObserver->TouchButtonPressed( iId );
            return ETrue;
            }
        return EFalse;
        }
    return EFalse;
	}


