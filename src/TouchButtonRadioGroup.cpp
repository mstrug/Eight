/*
 ============================================================================
 Name		: TouchButtonRadioGroup.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CTouchButtonRadioGroup implementation
 ============================================================================
 */

#include "TouchButtonRadioGroup.h"
#include "EightSettings.h"
#include "TouchButtonObserver.h"
#include <vg/vgu.h>


const VGfloat KBH = KButtonHeight;
const VGfloat KBW = KBH;
const VGfloat KBM = 20;



CTouchButtonRadioGroup::CTouchButtonRadioGroup( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, TUint aLabelFontId, TUint aItemFontId, CVgFontManager& aFontManager ) :
    CTouchControlBase( aObserver, aId, aButtonRect ), iLabelFontId( aLabelFontId ), iButtonFontId( aItemFontId ), iFontManager( aFontManager )
    {
    }

CTouchButtonRadioGroup::~CTouchButtonRadioGroup()
    {
    iLabelText.Close();
    
    vgDestroyPath( iPath );
    vgDestroyPaint( iPaint );
    vgDestroyPaint( iPaintSel );
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ].iText.Close();
        vgDestroyPath( iButtons[ i ].iPath );
        }
    iButtons.Close();
    }

CTouchButtonRadioGroup* CTouchButtonRadioGroup::NewL( MTouchButtonObserver* aObserver, TUint aId, TVgRect aButtonRect, const TDesC& aLabelText, TUint aLabelFontId, TUint aItemFontId, CVgFontManager& aFontManager )
    {
    CTouchButtonRadioGroup* self = new ( ELeave ) CTouchButtonRadioGroup( aObserver, aId, aButtonRect, aLabelFontId, aItemFontId, aFontManager );
    CleanupStack::PushL( self );
    self->ConstructL( aLabelText );
    CleanupStack::Pop(); // self;
    return self;
    }

void CTouchButtonRadioGroup::ConstructL( const TDesC& aLabelText )
    {
    iLabelText.CreateL( aLabelText );
    
    iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    
    iPaint = CEightSettings::GetInstance()->iPalette.iRadioButtonColor.CreateVgPaint();
    iPaintSel = CEightSettings::GetInstance()->iPalette.iMsgBoxButtonColor.CreateVgPaint();
    }

void CTouchButtonRadioGroup::AddButtonL( TUint aId, const TDesC& aButtonText )
    {
    SButton sb;
    sb.iPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    sb.iId = aId;
    sb.iText.CreateL( aButtonText );
    sb.iSelected = iButtons.Count() == 0;
    sb.iPressed = EFalse;
    
    iButtons.AppendL( sb );
    }

void CTouchButtonRadioGroup::SetSelectedButtonId( TUint aId )
    {
    UnselectAllButtons();
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ].iId == aId )
            {
            iButtons[ i ].iSelected = ETrue;
            UpdateGraphic( i );
            break;
            }
        }
    }

TInt CTouchButtonRadioGroup::SelectedButtonId() const
    {
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ].iSelected )
            {
            return iButtons[ i ].iId;
            }
        }
    return KErrNotFound;
    }

void CTouchButtonRadioGroup::SetRect( const TVgRect& aRect )
    {
    TVgRect r( aRect );
    UpdateRects( r );
//    if ( r.Height() < 10 ) r.iBr.iY = r.iTl.iY - 100;
    
    CTouchControlBase::SetRect( r );
    
    vgClearPath( iPath, VG_PATH_CAPABILITY_ALL );
    r.AddVguRect( iPath );
    }

void CTouchButtonRadioGroup::UpdateGraphic( TInt aIdx )
    {
    vgClearPath( iButtons[ aIdx ].iPath, VG_PATH_CAPABILITY_ALL );
    vguEllipse( iButtons[ aIdx ].iPath, iButtons[ aIdx ].iRect.Center().iX, iButtons[ aIdx ].iRect.Center().iY, iButtons[ aIdx ].iRect.Width(), iButtons[ aIdx ].iRect.Height() );
    if ( iButtons[ aIdx ].iSelected )
        {
        vguEllipse( iButtons[ aIdx ].iPath, iButtons[ aIdx ].iRect.Center().iX, iButtons[ aIdx ].iRect.Center().iY, iButtons[ aIdx ].iRect.Width() / 2, iButtons[ aIdx ].iRect.Height() / 2 );
        }
    }

void CTouchButtonRadioGroup::UnselectAllButtons()
    {
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ].iSelected )
            {
            iButtons[ i ].iSelected = EFalse;
            UpdateGraphic( i );
            }
        }
    }

void CTouchButtonRadioGroup::UpdateRects( TVgRect& aRect )
    {
    TVgRect r( aRect );
    r.iTl.iY -= 45; // label height
    r.iBr.iY = r.iTl.iY - KBH;
    r.iBr.iX = r.iTl.iX + KBW;

    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ].iRect = r;
        UpdateGraphic( i );
        r.Move( 0, - KBH - KBM );
        }
    
    aRect.iBr.iY = r.iBr.iY + KBH + KBM;
    }

void CTouchButtonRadioGroup::Draw()
    {
    if ( !iVisible )
        {
        return;
        }
    
    //vgSetPaint( iPaint2, VG_FILL_PATH );
    //vgDrawPath( iPath, VG_FILL_PATH );
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ].iPressed )
            {
            vgSetPaint( iPaintSel, VG_FILL_PATH );
            }
        else
            {
            vgSetPaint( iPaint, VG_FILL_PATH );
            }
        vgDrawPath( iButtons[ i ].iPath, VG_FILL_PATH );
        CVgFont* font = iFontManager.Get( iButtonFontId );
        if ( font )
            {
            TVgRect r( iButtons[ i ].iRect );
            r.iTl.iX = r.iBr.iX + 10;
            r.iBr.iX = iRect.iBr.iX;
            r.iBr.iY += 6;
            font->DrawText( r, iButtons[ i ].iText, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalCenter );        
            }
        }
    
    vgSetPaint( iPaint, VG_FILL_PATH );
    CVgFont* font = iFontManager.Get( iLabelFontId );
    if ( font )
        {
        TVgRect r( iRect );
        vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_MULTIPLY );
        font->DrawText( r, iLabelText, ESGEFontAlignHorizontalLeft, ESGEFontAlignVerticalTop );        
        vgSeti( VG_IMAGE_MODE, VG_DRAW_IMAGE_NORMAL );
        }
    }

void CTouchButtonRadioGroup::Update( TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
    {
    
    }

TBool CTouchButtonRadioGroup::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( !iVisible || !iEnable )
        {
        return EFalse;
        }
    
    if ( !iPressed && aPointerEvent.iType == TPointerEvent::EButton1Down ) 
        {
        for ( TInt i = 0; i < iButtons.Count(); i++ )
            {
            TVgRect r( iButtons[ i ].iRect );
            if ( r.Contains( aPointerEvent.iPosition ) )
                {
                iButtons[ i ].iPressed = ETrue;
                iPressed = ETrue;
                iP = aPointerEvent.iPosition;
                return ETrue;
                }
            }
        }
    else if ( iPressed && aPointerEvent.iType == TPointerEvent::EButton1Up )
        {
        iPressed = EFalse;
        for ( TInt i = 0; i < iButtons.Count(); i++ )
            {
            if ( iButtons[ i ].iPressed )
                {
                iButtons[ i ].iPressed = EFalse;
                iP = aPointerEvent.iPosition;
                TVgRect r( iButtons[ i ].iRect );
                r.Grow( 10, 10 );
                if ( r.Contains( aPointerEvent.iPosition ) )
                    {
                    UnselectAllButtons();
                    iButtons[ i ].iSelected = ETrue;
                    UpdateGraphic( i );
                    if ( iObserver ) iObserver->TouchButtonPressed( iId );
                    return ETrue;
                    }
                return EFalse;
                }
            }
        }
    return EFalse;
    }


