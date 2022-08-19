/*
 ============================================================================
 Name		: TouchButtonText.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchMenuItemBase implementation
 ============================================================================
 */

#include "TouchButtonText.h"
#include "EightSettings.h"
#include "TouchButtonObserver.h"
#include "vg/vgu.h"



CTouchButtonText::CTouchButtonText( MTouchButtonObserver* aObserver, TUint aId, TVgRect aTextRect, TVgRect aButtonRect, TUint aFontId, CVgFontManager& aFontManager ) 
    : CTouchControlBase( aObserver, aId, aButtonRect), iFontManager( aFontManager ), iTextRect( aTextRect ), iFontId( aFontId ), iBorderWidth( 2 )
	{
	}

CTouchButtonText::~CTouchButtonText()
	{
    iText.Close();
    vgDestroyPath( iPath );
    vgDestroyPaint( iPaint );
    vgDestroyPaint( iBackgroundPaint );
    vgDestroyPaint( iPressedTextPaint );
	if ( iTextPaint )
		{
		vgDestroyPaint( iTextPaint );
		}
    if ( iUnderlinedPath )
        {
        vgDestroyPath( iUnderlinedPath );
        }
	}

CTouchButtonText* CTouchButtonText::NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aTextRect, TVgRect aButtonRect, const TDesC& aText, TUint aFontId, CVgFontManager& aFontManager )
    {
    CTouchButtonText* self = new (ELeave) CTouchButtonText( aObserver, aId, aTextRect, aButtonRect, aFontId, aFontManager );
    CleanupStack::PushL(self);
    self->ConstructL( aText );
    CleanupStack::Pop(); // self;
    return self;
    }

void CTouchButtonText::ConstructL( const TDesC& aText )
	{
    iText.CreateL( aText );
    
    iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 8, 4, VG_PATH_CAPABILITY_ALL );
    vguRect( iPath, iRect.iTl.iX, iRect.iBr.iY, iRect.Width(), iRect.Height() );
    
    iPaint = vgCreatePaint();
    vgSetParameteri( iPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.iColor.Begin() );
    
    iPressedTextPaint = KVgColorBlack.CreateVgPaint();
	}

void CTouchButtonText::SetRect( const TVgRect& aRect )
	{
	TInt ofs = iRect.iBr.iY - iTextRect.iBr.iY;
	if ( ofs == 0 ) ofs = -KTextOffsetMedium;
	TVgRect r( aRect );
	if ( r.Width() > KButtonWidth )
		{
		r.iBr.iX = r.iTl.iX + KButtonWidth;
		}
	CTouchControlBase::SetRect( r );
	iTextRect = r;
	iTextRect.iBr.iY -= ofs;
	
    vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
    r.AddVguRect( iPath );
	}

void CTouchButtonText::SetBackgroundColor( TVgColor aColor )
    {
    vgDestroyPaint( iBackgroundPaint );
    iBackgroundPaint = aColor.CreateVgPaint();
    }

void CTouchButtonText::SetSelectedBackgroundColor( TVgColor aColor )
	{
    vgDestroyPaint( iPaint );
    iPaint = aColor.CreateVgPaint();
	}

void CTouchButtonText::SetTextColor( TVgColor aColor )
	{
	if ( iTextPaint )
		{
		vgDestroyPaint( iTextPaint );
		}
	iTextPaint = aColor.CreateVgPaint();
	}

void CTouchButtonText::SetBorderWidth( VGfloat aWidth )
    {
    iBorderWidth = aWidth;
    }

void CTouchButtonText::SetLinkStyle( TVgRect aRect, TVgColor aColor )
	{
    if ( !iLinkStyle )
        {
    	iLinkStyle = ETrue;
        
        iUnderlinedPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 8, 4, VG_PATH_CAPABILITY_ALL );
        vguLine( iUnderlinedPath, (TInt)aRect.iTl.iX, (TInt)aRect.iBr.iY, (TInt)aRect.iBr.iX, (TInt)aRect.iBr.iY );
        
        SetSelectedBackgroundColor( aColor );
        SetTextColor( aColor );
        }
	}

void CTouchButtonText::Draw()
	{
    if ( !iVisible )
        {
        return;
        }

    vgSetf( VG_STROKE_LINE_WIDTH, iBorderWidth );

    if ( iPressed )
        {
        vgSetPaint( iPaint, VG_STROKE_PATH );
        vgSetPaint( iPaint, VG_FILL_PATH );
        vgDrawPath( iPath, VG_STROKE_PATH | VG_FILL_PATH );
        }
    else
        {
        vgSetPaint( iPaint, VG_STROKE_PATH );
        if ( iBackgroundPaint )
            {
            vgSetPaint( iBackgroundPaint, VG_FILL_PATH );
            vgDrawPath( iPath, VG_STROKE_PATH | VG_FILL_PATH );
            }
        else
            {
            vgDrawPath( iPath, VG_STROKE_PATH );
            }
        }

    CVgFont* font = iFontManager.Get( iFontId );
    if ( font )
        {
        if ( iPressed )
            {
            vgSetPaint( iPressedTextPaint, VG_FILL_PATH | VG_STROKE_PATH );
            vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_MULTIPLY );
            font->DrawText( iTextRect, iText );
            
            vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL );
            if ( iLinkStyle )
                {
                vgSeti( VG_STROKE_LINE_WIDTH, 2 );
                vgDrawPath( iUnderlinedPath, VG_STROKE_PATH );
                }
            }
        else
            {
        	if ( iTextPaint )
        		{
                vgSetPaint( iTextPaint, VG_FILL_PATH | VG_STROKE_PATH );
                vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_MULTIPLY );
        		}
            font->DrawText( iTextRect, iText );
            
        	if ( iTextPaint )
        		{
                vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL );
        		}
            if ( iLinkStyle )
                {
                vgSeti( VG_STROKE_LINE_WIDTH, 2 );
                vgDrawPath( iUnderlinedPath, VG_STROKE_PATH );
                }
            }
        }
    
    if ( !iEnable )
    	{
    	DrawDisabledRect();
    	}
	}

void CTouchButtonText::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
	{
	}

TBool CTouchButtonText::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{ // todo: same code as CTouchButtonPath
    if ( !iVisible || !iEnable )
        {
        return EFalse;
        }
    TVgRect r( iRect );
    if ( !iPressed && aPointerEvent.iType == TPointerEvent::EButton1Down && r.Contains( aPointerEvent.iPosition ) )
        {
        iPressed = ETrue;
        //vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, KVgColorWhite.iColor.Begin() ); //CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.iColor.Begin() );

        iP = aPointerEvent.iPosition;
        return ETrue;
        }
    else if ( iPressed && aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        iPressed = EFalse;
        //vgSetParameterfv( iPaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.iColor.Begin() );

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

