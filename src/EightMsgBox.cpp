/*
 ============================================================================
 Name		: EightMsgBox.cpp
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : CEightMsgBox implementation
 ============================================================================
 */

#include "EightMsgBox.h"
#include "EightSettings.h"
#include <vg/vgu.h>

TInt Kbmh = KButtonMargin;
TInt Kbmw = Kbmh;
TInt Kbh = KButtonHeight;
TInt Kbw = KButtonWidth;
    

CEightMsgBox::CEightMsgBox( TSize aScreenSize, CVgFontManager& aFontManager, TInt aFontTitleId, TInt aFontTextId, TInt aFontButtonId )
    : iScreenSize( aScreenSize ), iFontManager( aFontManager ), iFontTitleId( aFontTitleId ), iFontTextId( aFontTextId ), iFontButtonId( aFontButtonId )
    {
    iBackgroundPath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    
    iBackgroundPaint = vgCreatePaint();
    vgSetParameteri( iBackgroundPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( iBackgroundPaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iMsgBoxBackgroundColor.iColor.Begin() );

    iFadePath = vgCreatePath( VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1, 0, 10, 20, VG_PATH_CAPABILITY_ALL );
    vguRect( iFadePath, 0, 0, iScreenSize.iWidth, iScreenSize.iHeight );

    iFadePaint = vgCreatePaint();
    vgSetParameteri( iFadePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR );
    vgSetParameterfv( iFadePaint, VG_PAINT_COLOR, 4, CEightSettings::GetInstance()->iPalette.iTilesFadeColor.iColor.Begin() );
    
#ifndef __WINS__  
    iTouchFeedback = MTouchFeedback::Instance();
#endif
    }

CEightMsgBox::~CEightMsgBox()
    {
    iMessage.Close();
    iObservers.Close();

    vgDestroyPath( iBackgroundPath );
    vgDestroyPaint( iBackgroundPaint );
    
    vgDestroyPath( iFadePath );
    vgDestroyPaint( iFadePaint );
    
    iButtons.ResetAndDestroy();
    }

void CEightMsgBox::RegisterObserver( MEightMsgBoxObserver* aObserver )
    {
    iObservers.AppendL( aObserver );
    }

void CEightMsgBox::UnregisterObserver( MEightMsgBoxObserver* aObserver )
    {
    for ( TInt i = 0; i < iObservers.Count(); i++ )
        {
        if ( iObservers[ i ] == aObserver )
            {
            iObservers.Remove( i );
            i--;
            }
        }
    }

void CEightMsgBox::SetBlock()
    {
    iBlock = ETrue;
    }

TBool CEightMsgBox::IsVisible() const
    {
    return iVisible;
    }

TUint CEightMsgBox::MessageId() const
    {
    return iMessageId;
    }

TBool CEightMsgBox::ShowL( TUint aMessageId, TMsgBoxButton aButtons, const TDesC& aMessage )
    {
    if ( iVisible || iBlock ) return EFalse;
    
    iVisible = ETrue;
    iMessageId = aMessageId;
    
    iMessage.Close();
    iMessage.CreateL( aMessage );
    iButtons.ResetAndDestroy();

    TInt nl = 1;
    for ( TInt i = 0; i < aMessage.Length(); i++ )
        {
        if ( aMessage[ i ] == '\n' ) nl++;
        }
    TInt bmh = Kbmh;
    TInt bmw = Kbmw;
    TInt bh = Kbh;
    TInt bw = Kbw; //( iScreenSize.iWidth - 3 * bmw ) / 2;
    iHeight = nl * 20 + 3 * bmh + bh;
    if ( nl == 1 ) iHeight += 10;

    vgClearPath( iBackgroundPath, VG_PATH_CAPABILITY_ALL );
    vguRect( iBackgroundPath, 0, iScreenSize.iHeight - iHeight, 360, iHeight );

    switch ( aButtons )
        {
        case EMsgBoxButtonOk:
            {
            TVgRect r2 = TVgRect( TVgPoint( iScreenSize.iWidth - bmw - bw, iScreenSize.iHeight - iHeight + bmh + bh ), TVgSize( bw, bh ) );
            TVgRect rt2 = r2; rt2.iBr.iY += KTextOffsetMedium;
            CTouchButtonText* b2 = CTouchButtonText::NewL( this, KMsgBoxButtonOk, rt2, r2, _L("ok"), iFontButtonId, iFontManager );
            iButtons.AppendL( b2 );
            }
            break;
        case EMsgBoxButtonYesNo:
            {
            TVgRect r1 = TVgRect( TVgPoint( bmw, iScreenSize.iHeight - iHeight + bmh + bh ), TVgSize( bw, bh ) );
            TVgRect rt1 = r1; rt1.iBr.iY += KTextOffsetMedium;
            CTouchButtonText* b1 = CTouchButtonText::NewL( this, KMsgBoxButtonYes, rt1, r1, _L("yes"), iFontButtonId, iFontManager );
            iButtons.AppendL( b1 );

            TVgRect r2 = TVgRect( TVgPoint( iScreenSize.iWidth - bmw - bw, iScreenSize.iHeight - iHeight + bmh + bh ), TVgSize( bw, bh ) );
            TVgRect rt2 = r2; rt2.iBr.iY += KTextOffsetMedium;
            CTouchButtonText* b2 = CTouchButtonText::NewL( this, KMsgBoxButtonNo, rt2, r2, _L("no"), iFontButtonId, iFontManager );
            iButtons.AppendL( b2 );
            }
            break;
        }
    
    iOffsetY = iHeight;
    
    if ( CEightSettings::GetInstance()->iUseTouchFeedback && iTouchFeedback )
        {
        iTouchFeedback->InstantFeedback( ETouchFeedbackPopUp );
        }
    
    for ( TInt i = 0; i < iObservers.Count(); i++ )
        {
        iObservers[ i ]->MsgBoxWillShow();
        }
    
    return ETrue;
    }

void CEightMsgBox::Hide()
    {
    iOffsetY = -iHeight;
    }

void CEightMsgBox::TouchButtonPressed( TUint aId )
    {
//    iObserver->MsgBoxAction( iMessageId, aId );
    iStoredTouchButtonId = aId;
    Hide();
//    iVisible = EFalse;
//    iButtons.ResetAndDestroy();
//    iMessage.Close();
    }

TBool CEightMsgBox::HandlePointerEventL( const TPointerEvent& aPointerEvent )
    {
    if ( iOffsetY != 0 )
        {
        return EFalse;
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
         {
         if ( iButtons[ i ]->HandlePointerEventL( aPointerEvent ) )
             {
             return ETrue;
             }
         }

    return EFalse;
    }

void CEightMsgBox::Draw()
    {
    if ( iOffsetY >= 0 )
        {
        vgSetPaint( iFadePaint, VG_FILL_PATH );
        vgDrawPath( iFadePath, VG_FILL_PATH );
        }

    if ( iOffsetY > 0 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgTranslate( 0, iOffsetY );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgTranslate( 0, iOffsetY );
        }
    else if ( iOffsetY < 0 )
        {
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_GLYPH_USER_TO_SURFACE );
        vgTranslate( 0, iHeight + iOffsetY );
        vgSeti( VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE );
        vgTranslate( 0, iHeight + iOffsetY );
        }
    
    vgSetPaint( iBackgroundPaint, VG_FILL_PATH );
    vgDrawPath( iBackgroundPath, VG_FILL_PATH );
    
    CVgFont* font1 = iFontManager.Get( iFontTextId );
    if ( font1 )
        {
        font1->DrawText( TVgPoint( Kbmw + 3, iScreenSize.iHeight - 20 - Kbmh ), iMessage, ETrue );
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ]->Draw();
        }
    }

void CEightMsgBox::Update( TTimeIntervalMicroSeconds32 aTimeInterval )
    {
    if ( iOffsetY != 0 )
        {
        iOffsetY /= 1.3;
        if ( Abs( iOffsetY ) < 1 )
            {
            if ( iOffsetY < 0 )
                {
                for ( TInt i = 0; i < iObservers.Count(); i++ )
                    {
                    iObservers[ i ]->MsgBoxAction( iMessageId, iStoredTouchButtonId );
                    }
                iVisible = EFalse;
                iButtons.ResetAndDestroy();
                iMessage.Close();
                }
            iOffsetY = 0;
            }
        }
    
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        iButtons[ i ]->Update( aTimeInterval );
        }
    }


